# Compiler and linker settings
CC = gcc
CFLAGS = -c -O3 -W -Wall -I./include
LFLAGS = -L./lib
LIBS = -llzg
RM = rm -f

# Benchmark configuration

# Files
PRG = csv2c_lzg
PRG_OBJS = csv2c_lzg.o
STATIC_LIB = ./lib/liblzg.a

.PHONY: all clean

# Master rule
all: $(PRG)

# Clean rule
clean:
	$(RM) $(PRG) $(PRG_OBJS)

# Program build rules
$(PRG): $(PRG_OBJS) $(STATIC_LIB)
	$(CC) $(LFLAGS) -o $@ $^ $(LZG_OBJS) $(LIBS)
	$(RM) $(PRG_OBJS)

# Object files build rules
csv2c_lzg.o: csv2c_lzg.c ./include/lzg.h
	$(CC) $(CFLAGS) $<


