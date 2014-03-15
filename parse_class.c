#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <elf.h>
#include "jdec.h"
#include "java.h"

extern
#include "globals.h"


int
validate_class_hdr(void)
{
	errmsg("validating class header\n");
	cl.magic = read_int();
	cl.minor_version = read_short();
	cl.major_version = read_short();
	if (cl.magic == 0xcafebabe) {
		errmsg("magic number OKAY\n");
		printf("major/minor versions = %d/%d\n",
			cl.major_version, cl.minor_version);
		return 1;
	}
	return 0;
}


static int
parse_constant_pool(void)
{
	int n;
	int dummy;
	byte *p = getbuff(cl.constant_pool_count * sizeof(byte *));
	cl.constant_pool = (byte **)p;
	// Iterate through the constants. Store a pointer to each of
	// them into the constant pool array.
	for (n = 1; n < cl.constant_pool_count; ++n) {
		int tag;
		byte *here = read_curpos();
		p = here;
		++p;
		tag = read_byte();
		switch (tag) {
		case CONSTANT_Methodref:
			errmsg("Methodref\n");
			dummy = read_int();
			break;
		case CONSTANT_Fieldref:
			errmsg("Fieldref\n");
			dummy = read_int();
			break;
		case CONSTANT_Class:
			errmsg("Class\n");
			dummy = read_short();
			break;
		case CONSTANT_NameAndType:
			errmsg("NameAndType\n");
			dummy = read_int();
			break;
		case CONSTANT_Utf8:
			errmsg("Utf8\n");
			dummy = read_short();
			skip_bytes(dummy);
			break;
		default:
			printf("Unknown tag %d\n", tag);
			abort();
		}
	}
	return 1;
}


static int parse_interfaces(void)
{
	cl.interfaces_count = read_short();
	printf("interface count = %d\n", cl.interfaces_count);

	if (cl.interfaces_count == 0)
		cl.interfaces = NULL;
	else {
		cl.interfaces = read_curpos();
		skip_bytes(2 * cl.interfaces_count);
	}
	return 1;
}


static int
parse_fields(void)
{
	cl.fields_count = read_short();
	printf("fields count = %d\n", cl.fields_count);
	
	return 0;
}


int
parse_class(void)
{
	// The class steam is open, and the first 8 bytes have been read.

	cl.constant_pool_count = read_short();
	printf("constant count = %d\n", cl.constant_pool_count);

	if (!parse_constant_pool())
		return 0;
	cl.access_flags = read_short();
	cl.this_class = read_short();
	cl.super_class = read_short();
	printf("access = %04x this = %d super = %d\n",
		cl.access_flags, cl.this_class, cl.super_class);
	if (!parse_interfaces())
		return 0;
	if (!parse_fields())
		return 0;

	return 1;
}

void
release_buffers(void)
{
	// Release any buffers we allocated to hold class file stuff
	freebuff(cl.constant_pool);
};
