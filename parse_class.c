#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <elf.h>
#include "jdec.h"
#include "java.h"

extern
#include "globals.h"


static void
debug_print(char *s)
{
	if (g.debug)
		puts(s);
}


int
validate_class_hdr(void)
{
	debug_print("validating class header");
	cl.magic = read_int();
	if (cl.magic == 0xcafebabe) {
		cl.minor_version = read_short();
		cl.major_version = read_short();
		debug_print("magic number OKAY");
		if (g.debug)
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
	byte **p;

	cl.constant_pool_count = read_short();
	assert(cl.constant_pool_count);
	if (g.debug)
		printf("constant count = %d\n", cl.constant_pool_count);

	cl.constant_pool = getbuff(cl.constant_pool_count * sizeof(byte *));
	if (g.debug)
		printf("const pool ptr = %p\n", cl.constant_pool);

	// Iterate through the constants. Store a pointer to each of
	// them into the constant pool array.
	p = (byte **)cl.constant_pool;
	for (n = 1; n < cl.constant_pool_count; ++n) {
		int tag;
		byte *here = read_curpos();
		*p = here;
		++p;
		tag = read_byte();
		switch (tag) {
		case CONSTANT_Methodref:
			debug_print("Methodref");
			(void)read_int();
			break;
		case CONSTANT_Fieldref:
			debug_print("Fieldref");
			(void)read_int();
			break;
		case CONSTANT_Class:
			debug_print("Class");
			(void)read_short();
			break;
		case CONSTANT_NameAndType:
			debug_print("NameAndType");
			(void)read_int();
			break;
		case CONSTANT_Utf8:
			debug_print("Utf8");
			skip_bytes(read_short());
			break;
		case CONSTANT_String:
			debug_print("String");
			(void)read_short();
			break;
		case CONSTANT_Integer:
			debug_print("Integer");
			(void)read_int();
			break;
		case CONSTANT_Float:
			debug_print("Float");
			(void)read_int();
			break;
		case CONSTANT_Long:
			debug_print("Long");
			(void)read_long();
			++n;
			*p = *(p - 1);
			++p;
			break;
		case CONSTANT_Double:
			debug_print("Double");
			(void)read_long();
			++n;
			*p = *(p - 1);
			++p;
			break;
		default:
			fprintf(stderr, "Unknown tag %d\n", tag);
			abort();
		}
	}
	return 1;
}


static int parse_interfaces(void)
{
	cl.interfaces_count = read_short();
	if (g.debug)
		printf("interface count = %d\n", cl.interfaces_count);

	if (cl.interfaces_count == 0)
		cl.interfaces = NULL;
	else {
		cl.interfaces = read_curpos();
		skip_bytes(2 * cl.interfaces_count);
	}
	return 1;
}


static void
skip_attributes(int count)
{
	int n;
	for (n = 1; n <= count; ++n) {
		int name = read_short();
		int len = read_int();
		if (g.debug)
			printf("   Attr %d: name = %d  size = %d\n",
				n, name, len);
		skip_bytes(len);
	}
}


static int
parse_fields(void)
{
	byte **p;
	int n;
	cl.fields_count = read_short();

	cl.fields = getbuff(cl.fields_count * sizeof(byte *));
	if (g.debug)
		printf("fields count = %d\n", cl.fields_count);
	p = (byte **)cl.fields;
	
	for (n = 0; n < cl.fields_count; ++n) {
		int flags, name, descr, attrs;
		byte *here = read_curpos();
		*p = here;
		++p;
		flags = read_short();
		name = read_short();
		descr = read_short();
		attrs = read_short();
		if (g.debug) {
			printf("Field %d:\n", n + 1);
			printf("   flags %04x name %d descr %d attrs %d\n",
				flags, name, descr, attrs);
		}
		if (attrs) {
			errmsg("ERROR: Cannot handle field attributes yet\n");
			return 0;
		}
	}
	return 1;
}


static int
parse_methods(void)
{
	int n;
	byte **p;
	cl.methods_count = read_short();

	cl.methods = getbuff(cl.methods_count * sizeof(byte *));
	if (g.debug)
		printf("method count = %d\n", cl.methods_count);
	p = (byte **)cl.methods;

	for (n = 0; n < cl.methods_count; ++n) {
		int flags, name, descr, attrs;
		byte *here = read_curpos();
		*p = here;
		++p;
		flags = read_short();
		name = read_short();
		descr = read_short();
		attrs = read_short();
		if (g.debug) {
			printf("Method %d:\n", n + 1);
			printf("   flags %04x name %d descr %d attrs %d\n",
				flags, name, descr, attrs);
		}
		if (attrs)
			skip_attributes(attrs);
	}

	return 1;
}


static int
parse_attributes(void)
{
	int n;
	byte **p;
	cl.attributes_count = read_short();
	cl.attributes = getbuff(cl.attributes_count * sizeof(byte *));
	p = (byte **)cl.attributes;

	if (g.debug)
		printf("attribute count = %d\n", cl.attributes_count);
	for (n = 1; n <= cl.attributes_count; ++n){
		int name, len;
		byte *here = read_curpos();
		*p = here;
		++p;
		name = read_short();
		len = read_int();
		skip_bytes(len);
		if (g.debug) {
			printf("Attribute %d:\n", n);
			printf("   name %d len %d\n", name, len);
		}
	}

	return 1;
}


int
parse_class(void)
{
	// The class steam is open, and the first 8 bytes have been read.

	if (!parse_constant_pool())
		return 0;
	cl.access_flags = read_short();
	cl.this_class = read_short();
	cl.super_class = read_short();
	if (g.debug)
		printf("access = %04x this = %d super = %d\n",
			cl.access_flags, cl.this_class, cl.super_class);
	if (!parse_interfaces())
		return 0;
	if (!parse_fields())
		return 0;
	if (!parse_methods())
		return 0;
	if (!parse_attributes())
		return 0;
	return 1;
}

void
release_buffers(void)
{
	// Release any buffers we allocated to hold class file stuff
	freebuff(cl.constant_pool);
	freebuff(cl.fields);
	freebuff(cl.methods);
	freebuff(cl.attributes);
};
