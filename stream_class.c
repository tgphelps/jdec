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
	assert(p == 0);
	p = class;
}


void
class_close(void)
{
	assert(p);
	p = 0;
}


byte *
read_curpos(void)
{
	return p;
}


void
skip_bytes(int n)
{
	p += n;
}


int
read_byte(void)
{
	int n = *p;
	++p;
	return n;
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


int64_t
read_long(void)
{
	
	int64_t i = read_int();
	int64_t j = read_int();
	return (i <<32) + j;
}
