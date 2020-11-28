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

# cc65 -Oirs --add-source src/$name.c

# ca65 src/crt0.s
# ca65 src/$name.s -g

# ld65 -C src/mmc1.cfg -o $name.nes src/*.o nes.lib -Ln build/labels.txt

# mv -f src/*.o build/
# mv -f src/$name.s build/
# mv -f $name.nes build/

# python3 fceux_symbols.py

make

$nesemupath build/$name.nes >/dev/null
