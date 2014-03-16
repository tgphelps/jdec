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
	int i;
	byte **p = (byte **)cl.constant_pool;
	//printf("constant_pool pointers\n");
	//rawdump((void *)cl.constant_pool, "", 32);
	// p -> first constant, maybe.
	for (i = 1; i < cl.constant_pool_count; ++i) {
		byte *constp = *p;
		int tag = constp[0];
		assert(tag < NUM_CONSTS);
		printf("Const %d: tag:%s ", i, const_type[tag]);
		switch (tag) {
		case CONSTANT_Utf8:
		default:
			putchar('\n');
		}
		++p;
	}
}
