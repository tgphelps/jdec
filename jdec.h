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


int parse_class(void);

int validate_class_hdr();
void disassemble(void);
void release_buffers(void);

void class_open(byte *);
byte *read_curpos(void);
int read_byte(void);
int read_short(void);
int read_int(void);
void skip_bytes(int n);

void do_show_test(void);
