#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include "jdec.h"


static struct stat stat_buff;

/* Returns the byte size of a file, or -1 if there is any problem with it. */

int
get_file_size(char *path)
{
	int n;

	// BUG: Check for a normal file.
	n = stat(path, &stat_buff);
	if (n == -1) {
		errmsg("could not stat arg 1\n");
		return -1;
	}
	if (!S_ISREG(stat_buff.st_mode)) {
		errmsg("arg 1 not a regular file\n");
		return -1;
	}
	return stat_buff.st_size;
}


unsigned char *
read_file(char *path, int size)
{
	FILE *fp;
	int count;
	unsigned char *buff;

	int in_size = size;
	if (in_size > 1000000) {
		errmsg("input file is too big\n");
		return NULL;
	}
	fp = fopen(path, "rb");
	if (fp == NULL) {
		errmsg("error opening input file");
		return NULL;
	}
	buff = getbuff(in_size);
	count = fread(buff, 1, in_size, fp);
	fclose(fp);
	if (count != in_size) {
		errmsg("error reading input file\n");
		freebuff(buff);
		return NULL;
	}
	return buff;
}
