#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "jdec.h"

/*
 * This file provides functions to read a class file as a stream
 * of n-byte strings.
 */

static byte * bp;


void
class_open(byte *class)
{
	assert(bp == 0);
	bp = class;
}


void
class_close(void)
{
	assert(bp);
	bp = 0;
}


byte *
read_curpos(void)
{
	assert(bp);
	return bp;
}


void
skip_bytes(int n)
{
	assert(bp);
	bp += n;
}


int
read_byte(void)
{
	assert(bp);
	int n = *bp;
	++bp;
	return n;
}


int
read_short(void)
{
	assert(bp);
	int n = (*bp << 8) + *(bp + 1);
	bp += 2;
	return n;
}


int
read_int(void)
{
	int i = read_short();
	int j = read_short();
	return (i << 16) + j;
}


float
read_float(void)
{
	union {
		char c[4];
		float n;
	} u;
	u.c[3] = read_byte();
	u.c[2] = read_byte();
	u.c[1] = read_byte();
	u.c[0] = read_byte();
	return u.n;
}


double
read_double(void)
{
	union {
		char c[8];
		double n;
	} u;
	u.c[7] = read_byte();
	u.c[6] = read_byte();
	u.c[5] = read_byte();
	u.c[4] = read_byte();
	u.c[3] = read_byte();
	u.c[2] = read_byte();
	u.c[1] = read_byte();
	u.c[0] = read_byte();
	return u.n;
}


int64_t
read_long(void)
{
	
	int64_t i = read_int();
	int64_t j = read_int();
	return (i <<32) + j;
}



int
get_short(byte *p)
{
	int n = (*p << 8) + *(p + 1);
	return n;
}


int
get_int(byte *p)
{
	int i = get_short(p);
	int j = get_short(p + 2);
	return (i << 16) + j;
}
