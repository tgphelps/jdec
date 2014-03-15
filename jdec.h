#include <stdint.h>

typedef unsigned char byte;

#define errmsg(msg) fputs(msg, stderr);

extern int parse_cmdline(int argc, char *argv[]);
extern void abort(void);
extern void fatal_err(char *msg);
extern void usage(void);

extern void *getbuff(size_t size);
extern void freebuff(void *p);
extern void buffer_summary(void);

extern int get_file_size(char *path);

extern unsigned char *read_file(char *path, int size);

void hexdump(FILE *fp, void *buff, int len, int hex_only);

void rawdump(unsigned char *buff, char *fileout, int size);


int dissect_class(void);

int validate_class_hdr(byte *p);
void disassemble(void);

int read_short(byte *p);
int read_int(byte *p);
