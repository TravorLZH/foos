#include <cpu/acpi.h>
#include <cpu/memory.h>
#include <dev/serial.h>
#include <string.h>

static int checksum_valid(void *ptr,size_t size)
{
	int i;
	char *bytes=(char*)ptr;
	uint8_t sum;
	for(i=0;i<size;i++)
		sum+=bytes[i];
	return sum==0;
}

static struct acpi_sdt_header *find_fadt(struct acpi_rsdt *rsdt)
{
	int i;
	int n_sdt=rsdt->header.len - sizeof(struct acpi_sdt_header);
	n_sdt /= sizeof(void*);
	serial_printf("[acpi] There are %d entries in the RSDT\n",n_sdt);
	for(i=0;i<n_sdt;i++){
		if(!memcmp("FACP",rsdt->sdt[i]->signature,
					sizeof(rsdt->sdt[i]->signature)))
			return rsdt->sdt[i];
	}
	return NULL;
}

void handle_fadt(struct acpi_fadt *fadt)
{
	serial_printf("[acpi] smi=0x%x, acpi_enable=0x%d\n",fadt->century,
			fadt->acpi_enable);
}

int acpi_init(struct acpi_rsdp *rsdp)
{
	struct acpi_sdt_header *fadt;
	if(!checksum_valid(rsdp,sizeof(struct acpi_rsdp))){
		serial_print("[acpi] invalid RSDP! no ACPI\n");
		return 1;
	}
	serial_printf("[acpi] RSDP at 0x%x is valid\n",rsdp);
	serial_print("[acpi] ******************************\n");
	serial_print("[acpi] RSDP signature: `");
	serial_write(rsdp->signature,sizeof(rsdp->signature));
	serial_print("'\n");
	serial_printf("[acpi] ACPI vendor: `");
	serial_write(rsdp->oem,sizeof(rsdp->oem));
	serial_printf("'\n[acpi] ACPI version: %s\n",rsdp->revision==2?
			"2.0" : "1.0");
	serial_print("[acpi] ******************************\n");
	if(!checksum_valid(rsdp->rsdt,rsdp->rsdt->header.len)){
		serial_print("[acpi] invalid RSDT! no ACPI\n");
		return 1;
	}
	serial_printf("[acpi] RSDT at 0x%x is valid\n",rsdp->rsdt);
	if((fadt=find_fadt(rsdp->rsdt))==NULL){
		serial_print("[acpi] cannot find FADT! no ACPI\n");
		return 1;
	}
	handle_fadt((struct acpi_fadt*)fadt);
	return 0;
}
