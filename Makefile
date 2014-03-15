
CFLAGS = -Wall

OBJS = jdec.o parse_cmdline.o util.o hexdump.o rawdump.o buffer.o disassemble.o read_file.o dissect_class.o

disasm: $(OBJS)
	gcc -o jdec $(OBJS)

clean:
	-rm *.o jdec
