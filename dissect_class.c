#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <elf.h>
#include "jdec.h"

extern
#include "globals.h"


int validate_class_hdr(byte *p)
{
	errmsg("validating class header\n");
	cl.magic = read_int(p);
	cl.minor_version = read_short(p + 4);
	cl.major_version = read_short(p + 6);
	if (cl.magic == 0xcafebabe) {
		errmsg("magic number OKAY\n");
		printf("major/minor versions = %d/%d\n",
			cl.major_version, cl.minor_version);
		return 1;
	}
	return 0;
}

int
dissect_class(void)
{
	cl.constant_pool_count = read_short(g.pclass + 8);
	printf("constant count = %d\n", cl.constant_pool_count);
	return 1;
}
