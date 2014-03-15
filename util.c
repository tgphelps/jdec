#include <stdio.h>
#include <stdlib.h>
#include "jdec.h"

void
abort(void)
{
	errmsg("abort\n");
	exit(1);
}

void
fatal_err(char *msg)
{
	errmsg(msg);
	abort();
}

static char usage_msg[] =
	"usage: jdec [-h -r -c n --offset n -o src] elf\n";
void
usage(void)
{
	fputs(usage_msg, stderr);
}

int
read_short(byte *p)
{
	return (*p << 8) + *(p + 1);
}

int
read_int(byte *p)
{
	int i = read_short(p);
	return (i << 16) + read_short(p + 2);
}
