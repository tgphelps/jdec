struct {
	/* parsing stuff */
	char *file_in;
	char *file_out;
	char *section_arg;
	int byte_count;
	int offset;
	uint8_t got_fileout;
	uint8_t show_help;
	uint8_t disassemble;
	uint8_t show_elf_hdr;
	uint8_t show_section_tbl;
	uint8_t raw_dump;

	/* Data for class functions */
	int class_size;
	byte *pclass;
	
} g;
