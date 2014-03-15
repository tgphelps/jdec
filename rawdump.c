#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include "jdec.h"


void
rawdump(unsigned char *buff, char *fileout, int size)
{
	FILE *fpout;

	if (fileout[0]) {
		fpout = fopen(fileout, "wt");
		if (!fpout) {
			errmsg("failed to open output file\n");
			abort();
		}
	} else
		fpout = stdout;

	hexdump(fpout, buff, size,0);

	if (fpout != stdout)
		fclose(fpout);
}
