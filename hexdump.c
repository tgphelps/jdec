#include <stdio.h>

#define TESTING 0

#define LINE_SIZE 16

void dumpline(FILE *fp, unsigned char *buff, int size, int hex_only)
{
int i, adj;

for (i = 0; i < size; ++i)
	fprintf(fp, "%02x ", buff[i]);
	adj = 3 * (LINE_SIZE - size) + 2;
	for (i = 0; i < adj; ++i)
		fprintf(fp, " ");
	if (!hex_only) {
		for (i = 0; i < size; ++i) {
			unsigned char c;
			c = buff[i];
			if (c < 32 || c > 126)
				c = '.';
			fputc(c, fp);
		}
	}
	fputc('\n', fp);
}

void hexdump(FILE *fp, void *buff, int len, int hex_only)
{
	int offset = 0;

	while (offset < len) {
		int next_len = len - offset;
		if (next_len > 16)
			next_len = 16;
		dumpline(fp, buff + offset, next_len, hex_only);
		offset += next_len;
	}
}

#if TESTING
int main()
{
	FILE *fpin;
	FILE *fpout;
	unsigned char buff[512];

	fpin = fopen("hexdump.o", "rb");
	fpout = fopen("OUT", "wt");
	fread(buff, 1, 512, fpin);
	hexdump(fpout, buff, 512);
	fclose(fpin);
	fclose(fpout);
}
#endif
