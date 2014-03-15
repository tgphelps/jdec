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
