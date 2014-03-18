#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "jdec.h"
#include "java.h"

extern
#include "globals.h"


struct utf8_info {
	int len;
	char *cp;
};

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
	"NameAndType"
};


static void
print_utf8(struct utf8_info *s)
{
	int i;
	for (i = 0; i < s->len; ++i)
		putchar(s->cp[i]);
}


/* Return a point to the constant pool entry for constant <n>. */

static byte *
get_constant(int n)
{
	byte **p = (byte **)cl.constant_pool;
	return *(p + n - 1);
	return 0;
}


static void
get_utf8_string(int n, struct utf8_info *s)
{
	byte *bcon = get_constant(n);
	char *ccon = (char *)bcon;
	assert(*bcon == CONSTANT_Utf8);
	s->len = get_short(bcon + 1);
	s->cp = ccon + 3;
}


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
		struct utf8_info s;
		// I don't like using this class_open() thing,
		// but it works just fine for now.
		class_open(*p);
		++p;
		int tag = read_byte();
		assert(tag < NUM_CONSTS);
		printf("Const %d: tag:%s ", i, const_type[tag]);
		switch (tag) {
			int n1, n2;
		case CONSTANT_Utf8:
			len = read_short();
			printf("len: %d \"", len);
			cp = (char *)read_curpos();
			for (j = 0; j < len; ++j)
				putchar(*cp++);
			putchar('"');
			putchar('\n');
			break;
		case CONSTANT_Class:
		case CONSTANT_String:
			printf("str: \"");
			get_utf8_string(read_short(), &s);
			print_utf8(&s);
			putchar('"');
			putchar('\n');
			break;
		case CONSTANT_Fieldref:
		case CONSTANT_Methodref:
		case CONSTANT_InterfaceMethodref:
			n1 = read_short();
			n2 = read_short();
			printf("class: %d name_and_type: %d\n", n1, n2);
			break;
		case CONSTANT_NameAndType:
			printf("name: ");
			get_utf8_string(read_short(), &s);
			print_utf8(&s);
			printf(" type: ");
			get_utf8_string(read_short(), &s);
			print_utf8(&s);
			putchar('\n');
			break;
		case CONSTANT_Long:
			//++i;
			c_long = read_long();
			printf("value: %ld\n", c_long);
			break;
		case CONSTANT_Double:
			//++i;
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
	struct utf8_info s;
	byte **p = (byte **)cl.fields;
	printf("FIELDS\n");
	for (n = 1; n <= cl.fields_count; ++n) {
		int flags, attrs;
		byte *ap = *p;
		++p;
		printf("Field %d: name:", n);

		get_utf8_string(get_short(ap + 2), &s);
		print_utf8(&s);
		printf(" descr:");
		get_utf8_string(get_short(ap + 4), &s);
		print_utf8(&s);

		flags = get_short(ap);
		attrs = get_short(ap + 6);

		printf(" flags:%04x attrs:%d\n", flags, attrs);
	}
}


void
show_methods(void)
{
	int n;
	struct utf8_info s;
	byte **p = (byte **)cl.methods;
	printf("METHODS\n");
	for (n = 1; n <= cl.methods_count; ++n) {
		int flags, attrs;
		byte *ap = *p;
		++p;
		printf("Method %d: name:", n);

		get_utf8_string(get_short(ap + 2), &s);
		print_utf8(&s);

		printf(" descr:");
		get_utf8_string(get_short(ap + 4), &s);
		print_utf8(&s);

		flags = get_short(ap);
		attrs = get_short(ap + 6);
		printf(" flags:%04x attrs:%d\n", flags, attrs);
	}
}


void
show_attributes(void)
{
	int n;
	struct utf8_info s;
	byte **p = (byte **)cl.attributes;
	printf("ATTRIBUTES\n");
	for (n = 1; n <= cl.attributes_count; ++n) {
		int len;
		byte *ap = *p;
		++p;
		//printf("p = %p  ap = %p\n", p, ap);
		printf("Attribute %d: name:", n);

		get_utf8_string(get_short(ap), &s);
		print_utf8(&s);
		len = get_int(ap + 2);
		printf(" length:%d\n", len);
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
