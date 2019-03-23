#include <dev/pci.h>
#include <asm/ioports.h>

uint32_t pci_addr(uint8_t _bus,uint8_t _dev,uint8_t _func)
{
	uint32_t bus=(uint32_t)_bus;
	uint32_t dev=(uint32_t)_dev;
	uint32_t func=(uint32_t)_func;

	return (bus << 16) | (dev << 11) | (func << 8) | 0x80000000;
}

uint32_t pci_read(uint32_t addr,uint8_t off)
{
	outl(PCI_ADDR,addr | (off & 0xFC));
	return inl(PCI_DATA);
}

uint8_t pci_headertype(uint32_t addr)
{
	return (pci_read(addr,0xC) >> 16) & 0xFF;
}

uint16_t pci_device_id(uint32_t addr)
{
	return pci_read(addr,0) >> 16;
}

uint16_t pci_vendor_id(uint32_t addr)
{
	return pci_read(addr,0) & 0xFFFF;
}
