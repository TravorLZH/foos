#include <asm/ioports.h>
#include <cpu/acpi.h>
#include <cpu/memory.h>
#include <dev/serial.h>
#include <dev/pit.h>
#include <stdio.h>
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
	serial_printf("[acpi] Available SDT:\n",n_sdt);
	for(i=0;i<n_sdt;i++){
		serial_print("[acpi]\t");
		serial_write(rsdt->sdt[i]->signature,
				sizeof(rsdt->sdt[i]->signature));
		serial_send('\n');
	}
	for(i=0;i<n_sdt;i++){
		if(!memcmp("FACP",rsdt->sdt[i]->signature,
					sizeof(rsdt->sdt[i]->signature)))
			return rsdt->sdt[i];
	}
	return NULL;
}

static void acpi_enable(struct acpi_fadt *fadt)
{
	int i;
	if(inw(fadt->pm1a_ctrlblk) & 1){
		printf("[acpi] already enabled\n");
		return;
	}
	if(fadt->smi_cmdport!=0 && fadt->acpi_enable!=0){
		serial_print("[acpi] enabling\n");
		outb(fadt->smi_cmdport,fadt->acpi_enable);
		for(i=0;i<300;i++){
			if(inw(fadt->pm1a_ctrlblk) & 1)
				break;
			pit_delay(10);
		}
		if(i<300)
			puts("[acpi] enabled!");
		else
			puts("[acpi] not enabled!");
	}
}

int acpi_init(struct acpi_rsdp *rsdp)
{
	struct acpi_sdt_header *fadt;
	if(!checksum_valid(rsdp,sizeof(struct acpi_rsdp))){
		puts("[acpi] invalid RSDP! no ACPI");
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
		puts("[acpi] invalid RSDT! no ACPI");
		return 1;
	}
	serial_printf("[acpi] RSDT at 0x%x is valid\n",rsdp->rsdt);
	if((fadt=find_fadt(rsdp->rsdt))==NULL){
		puts("[acpi] cannot find FADT! no ACPI");
		return 1;
	}
	acpi_enable((struct acpi_fadt*)fadt);
	return 0;
}
