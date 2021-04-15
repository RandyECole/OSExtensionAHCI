#include "pci.h"
#include "common.h"
#include "cio.h"

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
 
    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
    /* write out the address */
    __outl(0xCF8, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    tmp = (uint16_t)((__inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}

uint16_t getVendorID(uint8_t bus, uint8_t slot, uint8_t function) {
    return pciConfigReadWord(bus,slot,function,0);
}

uint8_t getHeaderType(uint8_t bus, uint8_t slot, uint8_t function) {
    return pciConfigReadWord(bus,slot,function,0x0E) & 0x00FF;
}

void checkFunction(uint8_t bus, uint8_t device, uint8_t function) {
    //If class & subclass == 01 and 06 save the device for setup later
    //ide is 01 and 01
    __cio_printf("\nv:%04x, ", pciConfigReadWord(bus, device, function, 0x00));
    __cio_printf("d:%04x, ", pciConfigReadWord(bus, device, function, 0x02));
    __cio_printf("c:%04x, ", pciConfigReadWord(bus, device, function, 0x0a));
    __cio_printf("h:%04x", pciConfigReadWord(bus, device, function, 0x0e));
}

void checkDevice(uint8_t bus, uint8_t device) {
    uint8_t function = 0;

    uint16_t vendorID = getVendorID(bus, device, function);
    if(vendorID == 0xFFFF) return;        // Device doesn't exist
    checkFunction(bus, device, function);
    uint8_t headerType = getHeaderType(bus, device, function);
    if( (headerType & 0x80) != 0) {
        /* It is a multi-function device, so check remaining functions */
        for(function = 1; function < 8; function++) {
            if(getVendorID(bus, device, function) != 0xFFFF) {
                checkFunction(bus, device, function);
            }
        }
    }
}

void enumeratePCIDevices(){
    uint16_t bus;
    uint8_t device;

    for(bus = 0; bus < 256; bus++) {
        for(device = 0; device < 32; device++) {
            checkDevice(bus, device);
        }
    }
}
