# This Makefile was largely templated off the one found at 
# https://github.com/cppchriscpp/nes-starter-kit/blob/master/makefile.
#

# ==== Tweaks ====
# These are the things I'd expect to tweak the most,
# and are the least impactful (so tweak away).

# The name of our ROM.
ROM_NAME = escape

EMULATOR = mednafen

# ==== Makefile Utility Functions ====
# This recursive wildcard function will help us tell Make to just make all the *.c files
# it finds. 
# See: https://stackoverflow.com/questions/2483182/recursive-wildcards-in-gnu-make/18258352#18258352
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))



# ==== General Settings ====
# This assumes that cc65 et. al are actually installed in your PATH.
CCOMPILER = cc65
ASSEMBLER = ca65
LINKER = ld65
CFLAGS = -Oirs --add-source
RM = rm -f

CONFIG_FILE = src/mmc1.cfg

SOURCE_C=$(strip $(call rwildcard, src/, *.c))
SOURCE_S=$(patsubst src/, build/, $(patsubst %.c, %.s, $(SOURCE_C)))
SOURCE_O=$(addprefix build/, $(notdir $(patsubst %.s, %.o, $(SOURCE_S))))
SOURCE_HEADERS=$(strip $(call rwildcard, src/, *.h))

SOURCE_DIRS=$(sort $(dir $(call rwildcard, src, %))) build

VPATH=$(SOURCE_DIRS)

# Not going to automate quite as much of the other functionality just yet.

# Cancel a few implicit rules:
%.o: %.s
%.o: %.c
%.s: %.c

.PRECIOUS: $(SOURCE_C) $(SOURCE_S) $(SOURCE_O) build/%.s

.PHONY: all run clean

# Master rule
all: build

# Build rule(s)
build: build/$(ROM_NAME).nes 

# Not the most clever: just recompile everything if a header changes.
build/%.s : %.c $(SOURCE_HEADERS)
	$(CCOMPILER) $(CFLAGS) $< -o $(patsubst %.o, %.s, $@)

build/%.o : build/%.s
	$(ASSEMBLER) $<

build/%.s : build/%.c
	$(CCOMPILER) $(CFLAGS) $< -o $(patsubst %.o, %.s, $@)

build/crt0.o :
	$(ASSEMBLER) src/crt0.s -o build/crt0.o

build/$(ROM_NAME).nes: build/crt0.o $(SOURCE_O)
	$(LINKER) -C $(CONFIG_FILE) -o build/$(ROM_NAME).nes build/*.o nes.lib -Ln build/labels.txt

clean:
	$(RM) build/*

run: build/$(ROM_NAME).nes
	$(EMULATOR) $< >/dev/null

