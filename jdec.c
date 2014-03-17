#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "jdec.h"

#include "globals.h"

#define MAX_FILE_SIZE 1000000

int
main(int argc, char *argv[])
{
	if (!parse_cmdline(argc, argv)) {
		usage();
		abort();
	}
	if (g.show_help) {
		usage();
		exit(0);
	}

	/* We are ready to try to read the entire file into memory. */

	g.class_size = get_file_size(g.file_in);
	if (g.class_size < 0)
		abort();

	if (g.class_size > MAX_FILE_SIZE) {
		errmsg("input file is too big\n");
		abort();
	}

	/* We have a valid file size. Read the entire file in one gulp. */
	g.pclass = read_file(g.file_in, g.class_size);
	if (g.pclass == NULL)
		abort();

	/* We do any raw dump BEFORE validating that it's a class file */
	if (g.raw_dump) {
		int size;
		if ((g.byte_count > 0) && (g.byte_count < g.class_size))
			size = g.byte_count;
		else
			size = g.class_size;
		if (g.got_fileout)
			rawdump(g.pclass + g.offset, g.file_out, size);
		else
			rawdump(g.pclass + g.offset, "", size);
	} else {

		class_open(g.pclass);

		if (!validate_class_hdr()) {
			fprintf(stderr, "%s is not a Java class file\n",
				g.file_in);
			abort();
		}
		if (!parse_class()) {
			fprintf(stderr, "Failed to parse the class file\n");
			abort();
		}
		class_close();

        	if (g.show_class) {
			int i = 0;
			int flag = 1;
			while (flag) {
				switch (g.show_arg[i++]) {
				case 'a':
					show_attributes();
					break;
				case 'c':
					show_constant_pool();
					break;
				case 'f':
					show_fields();
					break;
				case 'i':
					show_interfaces();
					break;
				case 'm':
					show_methods();
					break;
				case 'z':
					do_show_test();
					break;
				case 0:
					flag = 0;
					break;
				default:
					errmsg("Invalid show argument\n");
				}
			}
		}
		if (g.disassemble)
			disassemble();
		release_buffers();
	}
	freebuff(g.pclass);
	buffer_summary();
	return 0;
}
