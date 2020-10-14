#!/bin/sh

#  compile.sh
#  Castle Escape
#
#  Created by Zaccari Silverman on 6/10/20.
#  

export name="escape"
export nesemupath="mednafen"

#"/Applications/OpenEmu.app/Contents/MacOS/OpenEmu"

#"/Applications/Nesicide/nes-emulator.app/Contents/MacOS/nes-emulator"

export CC65_HOME="/usr/local/share/cc65"
# Important; otherwise, CC65 will not know where to find its includes.

cc65 -Oirs $name.c --add-source
# cc65 -O $name.c --add-source # The game has advanced far enough that this is now definitely slower than -Oirs
ca65 crt0.s
ca65 $name.s -g

# ld65 -C nrom_32k_horz.cfg -o $name.nes crt0.o $name.o nes.lib -Ln labels.txt
ld65 -C nrom_32k_8k_horz.cfg -o $name.nes crt0.o $name.o nes.lib -Ln labels.txt

#rm *.o

mv -f labels.txt build/
mv -f $name.s build/
mv -f $name.nes build/

python3 fceux_symbols.py

$nesemupath build/$name.nes >/dev/null
