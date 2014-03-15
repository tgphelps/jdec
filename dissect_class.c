#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <elf.h>
#include "jdec.h"

extern
#include "globals.h"


int validate_class_hdr(byte *p)
{
	int i;
	errmsg("validating class header\n");
	i = read_int(p);
	if (i == 0xcafebabe) {
		errmsg("magic number OKAY\n");
		return 1;
	}
	return 0;
}

int
dissect_class(void)
{
	errmsg("Class dissection goes here\n");
	return 0;
}
