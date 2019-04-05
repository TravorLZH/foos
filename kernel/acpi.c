#include <cpu/acpi.h>
#include <dev/serial.h>

static int checksum_valid(void *ptr,size_t size)
{
	int i;
	char *bytes=(char*)ptr;
	uint8_t sum;
	for(i=0;i<size;i++)
		sum+=bytes[i];
	return sum==0;
}

int acpi_init(struct acpi_rsdp *rsdp)
{
	int i;
	if(!checksum_valid(rsdp,sizeof(struct acpi_rsdp))){
		serial_print("[acpi] invalid RSDP! stop initializing acpi\n");
		return 1;
	}
	serial_printf("[acpi] RSDP at 0x%x is valid\n",rsdp);
	serial_print("[acpi] ******************************\n");
	serial_print("[acpi] RSDP signature: `");
	for(i=0;i<sizeof(rsdp->signature);i++)
		serial_send(rsdp->signature[i]);
	serial_print("'\n");
	serial_printf("[acpi] ACPI vendor: `");
	for(i=0;i<sizeof(rsdp->oem);i++)
		serial_send(rsdp->oem[i]);
	serial_print("'\n");
	serial_printf("[acpi] ACPI version: %s\n",rsdp->revision==2?
			"2.0" : "1.0");
	serial_printf("[acpi] RSDT address: 0x%x\n",rsdp->rsdt);
	serial_print("[acpi] ******************************\n");
	return 0;
}
