#include <asm/ioports.h>
#include <cpu/acpi.h>
#include <cpu/memory.h>
#include <dev/serial.h>
#include <dev/pit.h>
#include <stdio.h>
#include <string.h>

bool no_shutdown=true;
uint32_t SLP_TYPa;
uint32_t SLP_TYPb;
uint32_t SLP_EN=1<<13;
uint32_t PM1a_CNT;
uint32_t PM1b_CNT;

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

static int acpi_enable(struct acpi_fadt *fadt)
{
	int i;

	PM1a_CNT=fadt->pm1a_ctrlblk;
	PM1b_CNT=fadt->pm1b_ctrlblk;

	if(inw(PM1a_CNT) & 1){
		printf("[acpi] already enabled\n");
		return 0;
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
		else{
			puts("[acpi] not enabled!");
			return 1;
		}
	}
	return 0;
}

/* Look for pattern 0x08,"_S5_",0x12 or 0x08,"\_S5_",0x12 in AML zone */
static uint8_t *acpi_search_s5(uint8_t *aml,uint32_t aml_len)
{
	uint8_t *s5;
	uint32_t pos;
	while(s5=(uint8_t*)memmem(aml,aml_len,"_S5_",4)){
		pos=s5-aml;
		if(pos!=0 && pos+4<aml_len && (s5[-1]==0x08 || (pos!=1 && \
			s5[-1]=='\\' && s5[-2]==0x08)) && s5[4]==0x12)
			return s5;
		aml_len-=pos+4;
		aml=s5+4;
	}
	return NULL;
}

/* Collects I/O information needed to trigger an ACPI shutdown
 * See <https://forum.osdev.org/viewtopic.php?t=16990> for more details
 */
static int acpi_shutdown_init(struct acpi_fadt *fadt)
{
	struct acpi_sdt_header *dsdt;
	uint8_t *aml;
	uint32_t aml_len;
	uint8_t *s5;

	dsdt=(struct acpi_sdt_header *)fadt->dsdt;
	/* Search _S5_ in the AML space directly after the header */
	aml=(uint8_t*)(dsdt+1);
	aml_len=dsdt->len;
	if(!(s5=(uint8_t*)acpi_search_s5(aml,aml_len))){
		serial_printf("[acpi] `_S5_' not found\n");
		return 1;
	}
	serial_printf("[acpi] `_S5_' found\n");

	/* Having found _S5_, we need to collect SLP_TYPa and SLP_TYPb */

	/* ACPI Spec says this is 4 */
	serial_printf("[acpi] `_S5_' package size: %d\n",s5[6]);
	SLP_TYPa=s5[8] << 10;
	SLP_TYPb=s5[10] << 10;
	return 0;
}

int acpi_shutdown(void)
{
	serial_printf("[acpi] shutting down...\n");
	puts("Shutting down...");
	if(no_shutdown)
		return 1;

	outw(PM1a_CNT,SLP_TYPa | SLP_EN);
	if(PM1b_CNT)
		outw(PM1b_CNT,SLP_TYPb | SLP_EN);
	return 0;
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

	if(acpi_enable((struct acpi_fadt*)fadt))
		return 1;
	if(no_shutdown=acpi_shutdown_init((struct acpi_fadt*)fadt))
		serial_printf("[acpi] shutdown feature unavailable\n");
	return 0;
}
