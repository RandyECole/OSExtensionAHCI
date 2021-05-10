#include "ahci.h"
#include "pci.h"
#include "common.h"
#include "klib.h"
#include "cio.h"
#include "x86pic.h"
#include "support.h"

static HBA_MEM* _abar;
static HBA_PORT* _portsList[32];
static uint8_t _portsAvail;
static HDD_DEVICE_LIST _hddDevs;


// Find a free command list slot
static int find_cmdslot(HBA_PORT *port)
{
   // If not set in SACT and CI, the slot is free
   uint32_t slots = (port->sact | port->ci);
   for (int i=0; i<32; i++)
   {
      if ((slots&1) == 0)
         return i;
      slots >>= 1;
   }
   __cio_printf("Cannot find free command list entry\n");
   return -1;
}



bool_t getDrives(HBA_PORT *port, uint16_t *buf)
{
   port->is = (uint32_t) -1;     // Clear pending interrupt bits
   int spin = 0; // Spin lock timeout counter
   int slot = find_cmdslot(port);
   if (slot == -1)
      return false;

   HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
   cmdheader += slot;
   cmdheader->cfl = sizeof(FIS_REG_H2D)/sizeof(uint32_t);   // Command FIS size
   cmdheader->w = 0;    // Read from device
   cmdheader->prdtl = (uint16_t) 1;   // PRDT entries count
 
   HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba);
   __memset(cmdtbl, 0, sizeof(HBA_CMD_TBL) +
      (cmdheader->prdtl-1)*sizeof(HBA_PRDT_ENTRY));

   // Last entry
   cmdtbl->prdt_entry[0].dba = (uint32_t) buf;
   cmdtbl->prdt_entry[0].dbc = 511; // 512 bytes per sector
   cmdtbl->prdt_entry[0].i = 1;
 
   // Setup command
   FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);
 
   cmdfis->fis_type = FIS_TYPE_REG_H2D;
   cmdfis->c = 1; // Command
   cmdfis->command = ATA_CMD_IDENTIFY;
   cmdfis->device = 0;  // Master device
 
   // The below loop waits until the port is no longer busy before issuing a new command
   while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
   {
      spin++;
   }
   if (spin == 1000000)
   {
      __cio_printf("\nPort is hung");
      //return false;
   }
 
//002800400010000002007e000000003f000000003030514d3520303020202020202020202020202002000003322e00042020352b5145202020484d55444441524b204953202020202020202020202020202020202020202020202020801020200b0000010200000000070200001000289d80003f011000000000a00000070007ffffff03

   port->ci = 1<<slot;  // Issue command
 
   // Wait for completion
   while (1)
   {
      // In some longer duration writes, it may be helpful to spin on the DPS bit 
      // in the PxIS port field as well (1 << 5)
      if ((port->ci & (1<<slot)) == 0) 
         break;
      if (port->is & HBA_PxIS_TFES) // Task file error
      {
         __cio_printf("\nRead disk error");
         return false;
      }
   }
 
   // Check again
   if (port->is & HBA_PxIS_TFES)
   {
      __cio_printf("\nRead disk error");
      return false;
   }
 
   return true;
}

static bool_t ahci_write(HBA_PORT *port, uint32_t startl, uint32_t starth, uint32_t count, uint16_t *buf)
{
   port->is = (uint32_t) -1;     // Clear pending interrupt bits
   int spin = 0; // Spin lock timeout counter
   int slot = find_cmdslot(port);
   if (slot == -1)
      return false;
 
   HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
   cmdheader += slot;
   cmdheader->cfl = sizeof(FIS_REG_H2D)/sizeof(uint32_t);   // Command FIS size
   cmdheader->w = 1;    // Write to device
   cmdheader->prdtl = (uint16_t)((count-1)>>4) + 1;   // PRDT entries count
 
   HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba);
   __memset(cmdtbl, 0, sizeof(HBA_CMD_TBL) +
      (cmdheader->prdtl-1)*sizeof(HBA_PRDT_ENTRY));
 
   // 8K bytes (16 sectors) per PRDT
   int i = 0;
   for (; i<cmdheader->prdtl-1; i++)
   {
      cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
      cmdtbl->prdt_entry[i].dbc = 8*1024-1;  // 8K bytes (this value should always be set to 1 less than the actual value)
      cmdtbl->prdt_entry[i].i = 1;
      buf += 4*1024; // 4K words
      count -= 16;   // 16 sectors
   }
   // Last entry
   cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
   cmdtbl->prdt_entry[i].dbc = (count<<9)-1; // 512 bytes per sector
   cmdtbl->prdt_entry[i].i = 1;
 
   // Setup command
   FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);
 
   cmdfis->fis_type = FIS_TYPE_REG_H2D;
   cmdfis->c = 1; // Command
   cmdfis->command = ATA_CMD_WRITE_DMA_EX;
 
   cmdfis->lba0 = (uint8_t)startl;
   cmdfis->lba1 = (uint8_t)(startl>>8);
   cmdfis->lba2 = (uint8_t)(startl>>16);
   cmdfis->device = 1<<6;  // LBA mode
 
   cmdfis->lba3 = (uint8_t)(startl>>24);
   cmdfis->lba4 = (uint8_t)starth;
   cmdfis->lba5 = (uint8_t)(starth>>8);
 
   cmdfis->countl = count & 0xFF;
   cmdfis->counth = (count >> 8) & 0xFF;
 
   // The below loop waits until the port is no longer busy before issuing a new command
   while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
   {
      spin++;
   }
   if (spin == 1000000)
   {
      __cio_printf("\nPort is hung");
      //return false;
   }
 
   port->ci = 1<<slot;  // Issue command
 
   // Wait for completion
   while (1)
   {
      // In some longer duration writes, it may be helpful to spin on the DPS bit 
      // in the PxIS port field as well (1 << 5)
      if ((port->ci & (1<<slot)) == 0) 
         break;
      if (port->is & HBA_PxIS_TFES) // Task file error
      {
         __cio_printf("\nRead disk error");
         return false;
      }
   }
 
   // Check again
   if (port->is & HBA_PxIS_TFES)
   {
      __cio_printf("\nRead disk error");
      return false;
   }
 
   return true;
}

static bool_t ahci_read(HBA_PORT *port, uint32_t startl, uint32_t starth, uint32_t count, uint16_t *buf)
{
   port->is = (uint32_t) -1;     // Clear pending interrupt bits
   int spin = 0; // Spin lock timeout counter
   int slot = find_cmdslot(port);
   if (slot == -1)
      return false;
 
   HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
   cmdheader += slot;
   cmdheader->cfl = sizeof(FIS_REG_H2D)/sizeof(uint32_t);   // Command FIS size
   cmdheader->w = 0;    // Read from device
   cmdheader->prdtl = (uint16_t)((count-1)>>4) + 1;   // PRDT entries count
 
   HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba);
   __memset(cmdtbl, 0, sizeof(HBA_CMD_TBL) +
      (cmdheader->prdtl-1)*sizeof(HBA_PRDT_ENTRY));
 
   // 8K bytes (16 sectors) per PRDT
   int i = 0;
   for (; i<cmdheader->prdtl-1; i++)
   {
      cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
      cmdtbl->prdt_entry[i].dbc = 8*1024-1;  // 8K bytes (this value should always be set to 1 less than the actual value)
      cmdtbl->prdt_entry[i].i = 1;
      buf += 4*1024; // 4K words
      count -= 16;   // 16 sectors
   }
   // Last entry
   cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
   cmdtbl->prdt_entry[i].dbc = (count<<9)-1; // 512 bytes per sector
   cmdtbl->prdt_entry[i].i = 1;
 
   // Setup command
   FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);
 
   cmdfis->fis_type = FIS_TYPE_REG_H2D;
   cmdfis->c = 1; // Command
   cmdfis->command = ATA_CMD_READ_DMA_EX;
 
   cmdfis->lba0 = (uint8_t)startl;
   cmdfis->lba1 = (uint8_t)(startl>>8);
   cmdfis->lba2 = (uint8_t)(startl>>16);
   cmdfis->device = 1<<6;  // LBA mode
 
   cmdfis->lba3 = (uint8_t)(startl>>24);
   cmdfis->lba4 = (uint8_t)starth;
   cmdfis->lba5 = (uint8_t)(starth>>8);
 
   cmdfis->countl = count & 0xFF;
   cmdfis->counth = (count >> 8) & 0xFF;
 
   // The below loop waits until the port is no longer busy before issuing a new command
   while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
   {
      spin++;
   }
   if (spin == 1000000)
   {
      __cio_printf("\nPort is hung");
      //return false;
   }
 
   port->ci = 1<<slot;  // Issue command
 
   // Wait for completion
   while (1)
   {
      // In some longer duration reads, it may be helpful to spin on the DPS bit 
      // in the PxIS port field as well (1 << 5)
      if ((port->ci & (1<<slot)) == 0) 
         break;
      if (port->is & HBA_PxIS_TFES) // Task file error
      {
         __cio_printf("\nRead disk error");
         return false;
      }
   }
 
   // Check again
   if (port->is & HBA_PxIS_TFES)
   {
      __cio_printf("\nRead disk error");
      return false;
   }
 
   return true;
}

// Start command engine
static void start_cmd(HBA_PORT *port)
{
   // Wait until CR (bit15) is cleared
   while (port->cmd & HBA_PxCMD_CR)
      ;
 
   // Set FRE (bit4) and ST (bit0)
   port->cmd |= HBA_PxCMD_FRE;
   //port->cmd |= HBA_PxCMD_CLO;
   port->cmd |= HBA_PxCMD_ST; 
}
 
// Stop command engine
static void stop_cmd(HBA_PORT *port)
{
   // Clear ST (bit0)
   port->cmd &= ~HBA_PxCMD_ST;
 
   // Clear FRE (bit4)
   port->cmd &= ~HBA_PxCMD_FRE;
 
   // Wait until FR (bit14), CR (bit15) are cleared
   while(1)
   {
      if (port->cmd & HBA_PxCMD_FR)
         continue;
      if (port->cmd & HBA_PxCMD_CR)
         continue;
      break;
   }
 
}

static void port_rebase(HBA_PORT *port, int portno)
{
   stop_cmd(port);   // Stop command engine
 
   // Command list offset: 1K*portno
   // Command list entry size = 32
   // Command list entry maxim count = 32
   // Command list maxim size = 32*32 = 1K per port
   port->clb = AHCI_BASE + (portno<<10);
   port->clbu = 0;
   __memset((void*)(port->clb), 0, 1024);
 
   // FIS offset: 32K+256*portno
   // FIS entry size = 256 bytes per port
   port->fb = AHCI_BASE + (32<<10) + (portno<<8);
   port->fbu = 0;
   __memset((void*)(port->fb), 0, 256);
 
   // Command table offset: 40K + 8K*portno
   // Command table size = 256*32 = 8K per port
   HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(port->clb);
   for (int i=0; i<32; i++)
   {
      cmdheader[i].prdtl = 8; // 8 prdt entries per command table
               // 256 bytes per command table, 64+16+48+16*8
      // Command table offset: 40K + 8K*portno + cmdheader_index*256
      cmdheader[i].ctba = AHCI_BASE + (40<<10) + (portno<<13) + (i<<8);
      cmdheader[i].ctbau = 0;
      __memset((void*)cmdheader[i].ctba, 0, 256);
   }
 
   start_cmd(port);  // Start command engine
}

static void _ahci_isr(int vector, int code)
{
   __cio_printf("\nIN AHCI ISR\n");
   uint32_t pi = _abar->pi;
   uint32_t is = _abar->is;
   for(uint8_t i = 0; i < 32; i++){
      if(pi & 1 && is & 1){
         _abar->pi |= 1 << i;
         uint32_t port_is = _abar->ports[i].is;
         
         //check port
         //writeback
         //if error bit set, reset port/retry commands as necessary.
         //Compare issued commands register to the commands you have recorded as issuing. For any bits where a command was issued but is no longer running, this means that the command has completed.
      }
      pi >>= 1;
      is >>= 1;
   }
   __outb( PIC_PRI_CMD_PORT, PIC_EOI );
}

static int check_type(HBA_PORT *port)
{
   uint32_t ssts = port->ssts;
 
   uint8_t ipm = (ssts >> 8) & 0x0F;
   uint8_t det = ssts & 0x0F;
 
   if (det != HBA_PORT_DET_PRESENT) // Check drive status
      return AHCI_DEV_NULL;
   if (ipm != HBA_PORT_IPM_ACTIVE)
      return AHCI_DEV_NULL;
 
   switch (port->sig)
   {
   case SATA_SIG_ATAPI:
      return AHCI_DEV_SATAPI;
   case SATA_SIG_SEMB:
      return AHCI_DEV_SEMB;
   case SATA_SIG_PM:
      return AHCI_DEV_PM;
   default:
      return AHCI_DEV_SATA;
   }
}


HDD_DEVICE_LIST getDevLst()
{
   return _hddDevs;
}

bool_t writeDisk(HDD_DEVICE device, uint32_t startl, uint32_t starth, uint32_t count, uint16_t *buf)
{
   return ahci_write(device.port, startl, starth, count, buf);
}

bool_t readDisk(HDD_DEVICE device, uint32_t startl, uint32_t starth, uint32_t count, uint16_t *buf)
{
   return ahci_read(device.port, startl, starth, count, buf);
}


void _ahci_init()
{
   //find device
   enumeratePCIDevices();
   _abar = (HBA_MEM*)getController().address;

   //enable interrupts(0@10) and memory space(1@1)

   //check if device supports legacy modes
   if((_abar->cap & 0x00040000) == 0){
      //set ahci enable
      __cio_printf("\nAHCI First Enable");
      _abar->ghc |= 0x80000000;
   }

   //Check if need to perform handoff
   if(_abar->cap2 & 0x0001){
      __cio_printf("\nBIOS/OS Handoff");
      //handoff
      //reset controller
      return;
   }
   else{
      __cio_printf("\nNo BIOS/OS Handoff");
   }

   //Install isr and given line
   __install_isr(PIC_PRI_CMD_PORT | getController().intLine, _ahci_isr);

   for(uint8_t i = 0; i < 32; i++){
      _abar->ports[i].cmd &= 0xFFFFFFEE;
   }

   _abar->ghc |= 0x01;

   while(_abar->ghc & 0x01){
      ;
   }

   if((_abar->cap & 0x00040000) == 0){
      //set ahci enable
      __cio_printf("\nAHCI Second Enable");
      _abar->ghc |= 0x80000000;
   }

   int o = 0;
   while(o < 700000){
      o++;
   }

   //enable interrupts
   _abar->ghc |= 0x00000002;

   //get ports and numbers
   __memset(_portsList, 32 * 4, 0);
   _portsAvail = 0;
   HBA_PORT* port = _abar->ports;
   uint32_t pi = _abar->pi;
   for(uint8_t i = 0; i < 32; i++){
      if(pi & 1){
         uint8_t deviceType = check_type(&port[i]);
         if(deviceType == AHCI_DEV_SATA){
            _portsList[_portsAvail++] = &port[i];
            __cio_printf("\nFound Sata Device at Port: %d", i);
         }
      }
      pi >>= 1;
   }
   for(uint8_t i = 0; i < _portsAvail; i++){
      port_rebase(_portsList[i], 0);
      int count = 0;
      while((_portsList[i]->ssts & 0xF) != 3 && count < 1000000){
         count++;
      }
      if(count == 1000000){
         __cio_printf("\nFail:");
         continue;
      }
      _portsList[i]->serr = 0xFFFFFFFF;
      _hddDevs.devices[count].port = _portsList[i];
      _hddDevs.count++;
   }

   // int* t = 0x600000;
   // __memset(t, 512, 255);
   // __cio_printf("\nBuff:");
   // for(int i = 0; i < 128; i++)
   //    __cio_printf("%08x", *(t + i));

   // __memset(t, 128*4, 255);
   // __cio_printf("\nSetBuff:%08x", *t);
   // readDisk(_hddDevs.devices[0], 2, 0, 1, 0x600000);
   // __cio_printf("\nRead:%08x", *t);
   // __memset(t, 128*4, 200);
   // __cio_printf("\nSetBuff:%08x", *t);
   // writeDisk(_hddDevs.devices[0], 2, 0, 1, 0x600000);
   // readDisk(_hddDevs.devices[0], 2, 0, 1, 0x600000);
   // __cio_printf("\nRead:%08x", *t);

   // getDrives(_portsList[0], 0x600000);
   // __cio_printf("\nBuff:");
   // IDENTIFY_DEVICE_DATA* dat = t;
   // __cio_printf("%s", dat->ModelNumber);
   // __cio_printf("%d   ", dat->Max48BitLBA[0]);
   // __cio_printf("%d", dat->Max48BitLBA[1]);
   // for(int i = 0; i < (512/4); i++)
   //    __cio_printf("%08x", *(t + i));

/*
For all the implemented ports:
Enable interrupts for the port. The D2H bit will signal completed commands.
Read signature/status of the port to see if it connected to a drive.
Send IDENTIFY ATA command to connected drives. Get their sector size and count. 
*/

}

