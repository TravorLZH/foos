#ifndef	ACPI_H
#define	ACPI_H

#include <inttypes.h>

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


extern int acpi_init(struct acpi_rsdp *rsdp_addr);

#endif
