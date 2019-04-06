#ifndef	ACPI_H
#define	ACPI_H

#include <foos/system.h>

/* System Description Table Header */
struct acpi_sdt_header {
	char signature[4];
	uint32_t len;
	uint8_t revision;
	uint8_t checksum;
	char oem[6];
	char oem_table[8];
	uint32_t oem_revision;
	uint32_t creator;
	uint32_t creator_revision;
} __attribute__((packed));

/* Root System Description Table */
struct acpi_rsdt {
	struct acpi_sdt_header header;
	struct acpi_sdt_header *sdt[];
};

/* RSDP: Root System Description Pointer */
struct acpi_rsdp {
	char signature[8];
	uint8_t checksum;
	char oem[6];
	uint8_t revision;
	struct acpi_rsdt *rsdt;
} __attribute__((packed));

/* Fixed ACPI Description Table */
struct acpi_fadt {
	struct acpi_sdt_header header;
	uint32_t firmware_ctrl;
	void *dsdt;
	uint8_t reserved;
	uint8_t ppm_profile;	// Preferred power management profile
	uint16_t sci_irq;	// System controll interrupt
	uint32_t smi_cmdport;
	uint8_t acpi_enable;
	uint8_t acpi_disable;
	uint8_t s4bios_req;
	uint8_t pstate_ctrl;
	uint32_t pm1a_evblk;	// PM1a Event Block
	uint32_t pm1b_evblk;
	uint32_t pm1a_ctrlblk;
	uint32_t pm1b_ctrlblk;
	uint32_t pm2_ctrlblk;
	uint32_t pm_timerblk;
	uint32_t gpe0_blk;
	uint32_t gpe1_blk;
	uint8_t pm1_evlen;	// PM1 Event length
	uint8_t pm1_ctrllen;
	uint8_t pm2_ctrllen;
	uint8_t pm_timerlen;
	uint8_t gpe0_len;
	uint8_t gpe1_len;
	uint8_t gpe1_base;
	uint8_t cstate_ctrl;
	uint16_t worst_c2latency;
	uint16_t worst_c3latency;
	uint16_t flush_size;
	uint16_t flush_stride;
	uint8_t duty_off;
	uint8_t duty_width;
	uint8_t day_alarm;
	uint8_t month_alarm;
	uint8_t century;
} __attribute__((packed));

extern int acpi_init(struct acpi_rsdp *rsdp_addr);

#endif
