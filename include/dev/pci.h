#ifndef	PCI_H
#define	PCI_H
#include <inttypes.h>

/* CONFIG ADDRESS SPECIFICATION
 * BIT 31:	Enable bit (always one)
 * BIT 30-24:	Reserved
 * BIT 23-16:	Bus no.
 * BIT 15-11:	Device no.
 * BIT 10-8:	Function no.
 * BIT 7-0:	Register offset (The first two bits are always zero)
 */

#define	PCI_ADDR	0xCF8	// PCI CONFIG ADDRESS (32-bit)
#define	PCI_DATA	0xCFC	// PCI CONFIG DATA (32-bit)

extern uint32_t pci_addr(uint8_t bus,uint8_t dev,uint8_t func);
extern uint32_t pci_read(uint32_t addr,uint8_t off);
extern uint8_t pci_headertype(uint32_t addr);
extern uint16_t pci_device_id(uint32_t addr);
extern uint16_t pci_vendor_id(uint32_t addr);

#endif
