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

struct {
	int magic;
	int minor_version;
	int major_version;
	int constant_pool_count;
	byte **constant_pool;
	int access_flags;
	int this_class;
	int super_class;
	int interfaces_count;
	byte *interfaces;
	int fields_count;
	byte *fields;
	int methods_count;
	byte *methods;
	int attributes_count;
	byte *attributes;
} cl;
