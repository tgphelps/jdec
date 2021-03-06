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
void class_close(void);
byte *read_curpos(void);
int read_byte(void);
int read_short(void);
int read_int(void);
float read_float(void);
int64_t read_long(void);
double read_double(void);
void skip_bytes(int n);
int get_short(byte *p);
int get_int(byte *p);

void do_show_test(void);
void show_constant_pool(void);
void show_interfaces(void);
void show_fields(void);
void show_methods(void);
void show_attributes(void);
