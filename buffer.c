#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "jdec.h"

/*
 * This module provides a wrapper around malloc/free. We put a header
 * and trailer around the user buffer to help catch buffer overwrites
 * and bad pointers to free() and multiple frees.
 */

#define HMAGIC ")HD("
#define TMAGIC ")TL("
#define MAXSIZE 1000000

/* 8 bytes */
struct header {
	char magic[4];
	unsigned int size;
};

/* 4 bytes */
struct trailer {
	char magic[4];
};

static int hsize = sizeof(struct header);
static int tsize = sizeof(struct trailer);

static unsigned int total_gets, total_frees;

void *
getbuff(size_t size)
{
	void *p;
	struct header *ph;
	struct trailer *pt;

	if (size > MAXSIZE) {
		errmsg("getbuff size too big");
		abort();
	}
	p = malloc(size + hsize + tsize);

	/* Build the header */
	ph = p;
	strncpy(ph->magic, HMAGIC, 4);
	ph->size = size;
	//hexdump(stderr, p, hsize, 0);

	/* Build the trailer */
	pt = p + hsize + size;
	strncpy(pt->magic, TMAGIC, 4);

	++total_gets;
	return p + hsize;
}


void
freebuff(void *p)
{
	struct header *ph;
	struct trailer *pt;

	p -= hsize;
	ph = p;
	if (strncmp(ph->magic, HMAGIC, 4)) {
		errmsg("HMAGIC bad\n");
		hexdump(stderr, p, hsize, 0);
		abort();
	}
	if (ph->size > MAXSIZE) {
		errmsg("freebuf size bad");
		abort();
	}
	pt = p + hsize + ph->size;
	if (strncmp(pt->magic, TMAGIC, 4)) {
		errmsg("TMAGIC bad\n");
		hexdump(stderr, (void *)pt, tsize, 0);
		abort();
	}
	strncpy(ph->magic, "FREE", 4);
	ph->size = 0;

	++total_frees;
	free(p);
}

void
buffer_summary(void)
{
/***
	fprintf(stderr, "DEBUG: buffer gets = %u, frees = %u\n",
		total_gets, total_frees);
***/
}
