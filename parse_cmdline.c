#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "jdec.h"

extern
#include "globals.h"

static struct option long_opts[] = {
	{"help", no_argument, 0, 'h'},
	{"raw", no_argument, 0, 'r'},
	{"disassemble", no_argument, 0, 'd'},
	{"count", required_argument, 0, 'c'},
	{"offset", required_argument, 0, 1},
	{"output", required_argument, 0, 'o'},
	{0, 0, 0, 0}
};

int
parse_cmdline(int argc, char *argv[])
{
	while (1) {
		int c;
		int opt_index = 0;
		c = getopt_long(argc, argv, "hdrc:o:",
			long_opts, &opt_index);
		if (c == -1) {
			/* if optind < argc, argv[optind] is 1st non-option*/
			break;
		}
		/* c = '?' if invalid */
		switch (c) {
		case '?':
			return 0;
		case 'h':
			g.show_help = 1;
			break;
		case 'd':
			g.disassemble = 1;
			break;
		case 'r':
			g.raw_dump = 1;
			break;
		case 'c':
			g.byte_count = atoi(optarg);
			break;
		case 1:
			g.offset = atoi(optarg);
			break;
		case 'o':
			g.got_fileout = 1;
			g.file_out = optarg;
			break;
		default:
			errmsg("ERROR: getopt unknown\n");
			return 0;
		}
	}
	// printf("argc %d optind %d\n", argc, optind);
	if (argc > optind + 1) {
		errmsg("too many parameters given\n");
		return 0;
	} else if (argc == optind + 1) {
		g.file_in = argv[optind];
	} else if (!g.show_help) {
		errmsg("no input file given\n");
		return 0;
	}
	return 1;
}
