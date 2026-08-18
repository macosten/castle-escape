# Castle Escape

Please note that this is very much a work in progress. 

## What is this?

This repository has the code for a NES ROM that contains ports of a bunch of Neopets minigames:
* [Escape from Meridell Castle](http://www.jellyneo.net/?go=escape_from_meridell_castle)
* [Hasee Bounce](https://www.jellyneo.net/?go=hasee_bounce_flash) (that I started before they announced it would be on the Switch game...)
* [Igloo Garage Sale - The Game](https://www.jellyneo.net/?go=igloo_garage_sale_2_flash)
* [Deckswabber](https://www.jellyneo.net/?go=deckswabber)

## Building

(If you're just looking to download the ROM, just go to the `build` folder and grab it from there. I haven't made any proper releases just yet!)

You will need cc65.

Run `compile.sh`, ensuring that `CC65_HOME` is set correctly for your machine. You may also want to tweak `nesemupath` so that an NES emulator automatically runs after the ROM compiles.

(If you're using Windows, either try using WSL or translate `compile.sh` back into `compile.bat`...)

Automatic generation of FCEUX symbols requires python3, but the build should still work even if this part fails.

## Future Plans

* The Petpetpet War (Mootix Drop, but different?)
* ...and more minigames until I run out of ROM space!

## About

cc65 isn't terrible... but I'd probably try using [llvm-mos](https://llvm-mos.org) if I were starting this today.

The code in this repository makes a bunch of concessions to the compiler in the name of optimization:
* Since cc65 actually pulls and pushes from the stack for variables allocated on the stack (instead of doing something like mapping it to some bit of memory), but that wastes CPU cycles, I use a bunch of global variables to speed up memory access.
* I use a bunch of lookup tables (this is probably good advice regardless of compiler) since those are often faster than doing some kind of calculation.
* Multiplication and division should be kept rare (on a per-frame basis). You're better off 

I could go on, but [this page](https://github.com/ilmenit/CC65-Advanced-Optimizations) will do a better job of documenting optimizations for cc65 than I can.

From what I can tell, llvm-mos lets you write that's a lot closer to "normal" C than the code you'll see here. My guess is that you would still need to do things like use lots of lookup tables or call assembly functions for certain tasks, though.

If you're just looking to make some kind of NES game... RetroGameForge will probably make it easier (and you get a more sophisticated mapper), once that comes out!

## Thanks to...

* nesdoug (for the [documentation](https://nesdoug.com) I used to start this repo!)
* rainwarrior (For originally writing `fceux_symbols.py`)
* Tuï (I modified some of their sound effects at some point for use in this!)
