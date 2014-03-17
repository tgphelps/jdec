#include <stdio.h>
#include <assert.h>
#include "jdec.h"
#include "java.h"

extern
#include "globals.h"


#define NUM_CONSTS 13

char *const_type[NUM_CONSTS] = {
	"ZERO",
	"Utf8",
	"TWO",
	"Integer",
	"Float",
	"Long",
	"Double",
	"Class",
	"String",
	"Field",
	"Method",
	"IfMethod",
	"NameAndtype"
};

void
do_show_test(void)
{
	errmsg("SHOW TEST\n");
}


// int cl.constant_pool_count
// byte **cl.constant_pool

void
show_constant_pool(void)
{
	int i, j;
	char *cp;
	int c_int;
	float c_float;
	double c_double;
	uint64_t c_long;

	byte **p = (byte **)cl.constant_pool;
	//printf("constant_pool pointers\n");
	//rawdump((void *)cl.constant_pool, "", 32);
	// p -> first constant, maybe.
	printf("CONSTANTS\n");
	for (i = 1; i < cl.constant_pool_count; ++i) {
		int len;
		//byte *constp = *p;
		//class_open(constp);
		class_open(*p);
		++p;
		int tag = read_byte();
		assert(tag < NUM_CONSTS);
		printf("Const %d: tag:%s ", i, const_type[tag]);
		switch (tag) {
			int n1, n2;
		case CONSTANT_Utf8:
			len = read_short();
			printf("len: %d ", len);
			cp = (char *)read_curpos();
			for (j = 0; j < len; ++j)
				putchar(*cp++);
			putchar('\n');
			break;
		case CONSTANT_Class:
		case CONSTANT_String:
			printf("index: %d\n", read_short());
			break;
		case CONSTANT_Fieldref:
		case CONSTANT_Methodref:
		case CONSTANT_InterfaceMethodref:
			n1 = read_short();
			n2 = read_short();
			printf("class: %d name_and_type: %d\n", n1, n2);
			break;
		case CONSTANT_NameAndType:
			n1 = read_short();
			n2 = read_short();
			printf("name: %d descr: %d\n", n1, n2);
			break;
		case CONSTANT_Long:
			// I THINK:
			++i;
			c_long = read_long();
			printf("value: %ld\n", c_long);
			break;
		case CONSTANT_Double:
			// I THINK:
			++i;
			c_double = read_double();
			printf("value: %f\n", c_double);
			break;
		case CONSTANT_Integer:
			c_int = read_int();
			printf("value: %d\n", c_int);
			break;
		case CONSTANT_Float:
			c_float = read_float();
			printf("value: %f\n", c_float);
			break;
		default:
			assert(0);
			printf("TODO");
			putchar('\n');
		}
		class_close();
		//++p;
	}
}


void
show_fields(void)
{
	int n;
	byte **p = (byte **)cl.fields;
	printf("FIELDS\n");
	for (n = 1; n <= cl.fields_count; ++n) {
		int flags, name, descr, attrs;
		class_open(*p);
		++p;
		flags = read_short();
		name = read_short();
		descr = read_short();
		attrs = read_short();
		printf("Field %d: flags:%04x name:%d descr:%d attrs:%d\n",
			n, flags, name, descr, attrs);
		class_close();
	}
}


void
show_methods(void)
{
	int n;
	byte **p = (byte **)cl.methods;
	printf("METHODS\n");
	for (n = 1; n <= cl.methods_count; ++n) {
		int flags, name, descr, attrs;
		class_open(*p);
		++p;
		flags = read_short();
		name = read_short();
		descr = read_short();
		attrs = read_short();
		printf("Method %d: flags:%04x name:%d descr:%d attrs:%d\n",
			n, flags, name, descr, attrs);
		class_close();
	}
}


void
show_attributes(void)
{
	int n;
	byte **p = (byte **)cl.attributes;
	printf("ATTRIBUTES\n");
	for (n = 1; n <= cl.attributes_count; ++n) {
		int name, len;
		class_open(*p);
		++p;
		name = read_short();
		len = read_int();
		printf("Attribute %d: name:%d length:%d\n", n, name, len);
		class_close();
	}
}


void
show_interfaces(void)
{
	if (cl.interfaces_count) {
		errmsg("Cannot show interfaces yet\n");
		abort();
	}
}
