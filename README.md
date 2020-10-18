# Castle Escape

## About:

This game is meant to (eventually) be a port of the Neopets game [Escape from Meridell Castle](http://www.jellyneo.net/?go=escape_from_meridell_castle).  

This project used [NESDoug's platformer](https://nesdoug.com) series as a starting point and has sort of branched off from there.

While I lack experience with cc65 and NES development in general, I imagine this isn't a terrible start...

Be warned that C targeted at cc65 (due to the nature of the 6502) is often abnormal: globals are encouraged (because the C stack is considered slow), lookup tables are commonplace (because multiplication/division except by powers of 2 is slow)... I could go on, but [this page](https://github.com/ilmenit/CC65-Advanced-Optimizations) will do a better job of documenting optimizations for cc65 than I can.

## Building

(If you're just looking to download the ROM, just go to the `build` folder and grab it from there.)

You will need cc65.

Run `compile.sh`, ensuring that `CC65_HOME` is set correctly for your machine. You may also want to tweak `nesemupath` so that an NES emulator automatically runs after the ROM compiles.

(If you're using Windows, either try using WSL or translate `compile.sh` back into `compile.bat`...)

Automatic generation of FCEUX symbols requires python3, but the build should still work even if this part fails.

## Future Challenges/Worries:

* Speed. cc65 isn't terrible, but it's not always the most performant. Some functions may be more performant if I compile them in an alternate compiler, or if I manually optimize them.
* Cannons in particular feel like they'll be tricky to implement fully.
* Stars/Collectables don't always disappear immediately when collected. I suspect this is because I'm missing something small/fundamental, but I haven't figured out what it is yet.

## Thanks to...

* nesdoug (For the documentation!)
* rainwarrior (For originally writing `fceux_symbols.py`)