#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "jdec.h"

/*
 * This file provides functions to read a class file as a stream
 * of n-byte strings.
 */

static byte * p;

void
class_open(byte *class)
{
	p = class;
}

int
read_short(void)
{
	int n = (*p << 8) + *(p + 1);
	p += 2;
	return n;
}

int
read_int(void)
{
	int i = read_short();
	int j = read_short();
	return (i << 16) + j;
}
