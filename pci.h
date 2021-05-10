#ifndef PCI_H_
#define PCI_H_

#include "common.h"

typedef struct ahci_controller_s {
   uint8_t bus;
   uint8_t device;
   uint8_t function;
   uint32_t address;
   uint8_t intPIN;
   uint8_t intLine;
} AHCI_CONTROLLER;

void enumeratePCIDevices( void );

AHCI_CONTROLLER getController( void );

#endif
