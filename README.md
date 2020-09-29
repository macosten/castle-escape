# Castle Escape

## About:

This game is meant to (eventually) be a port of the Neopets game [Escape from Meridell Castle](http://www.jellyneo.net/?go=escape_from_meridell_castle).  

This project used [NESDoug's platformer](https://nesdoug.com) series as a starting point and has sort of branched off from there.

While I lack experience with cc65 and NES development in general, I imagine this isn't a terrible start...

## Building

(If you're just looking to download the ROM, just go to the `build` folder and grab it from there.)

You will need cc65.

Run `compile.sh`, ensuring that `CC65_HOME` is set correctly for your machine. You may also want to tweak `nesemupath` so that an NES emulator automatically runs after the ROM compiles.

(If you're using Windows, either try using WSL or translate `compile.sh` back into `compile.bat`...)

Automatic generation of FCEUX symbols requires python3, but the build should still work even if this part fails.

## Future Challenges/Worries:

* Speed. gray_line() is too close to the bottom for my liking. Optimizations, either in C or in assembly, will need to happen more...
* So far I'm not using a mapper, but there's not *that* much space for level data. A mapper might be nice simply to offer way more space for levels; that, or I compress the level data somehow, which might be tricky...
* Cannons in particular feel like they'll be tricky to implement fully.
* Stars currently don't really work properly (they're background tiles that are supposed to clear when you touch them, but this doesn't happen consistently). Furthermore, they're supposed to also remain collected even when they are unloaded from memory, but I can't think of a very good way to implement this... but I also can't really make them sprites, because I'll run into the 8 sprites per scanline limit very quickly.


## Future Ideas:

cc65 is not that great (as far as code speed is concerned), but there are a couple of threads on the NESDev forums regarding newer C compilers. How difficult would it be to move over to one of these? If there's a speedup and I'm running out of CPU cycles, it'd probably be worth the hassle.

## Thanks to...

* nesdoug (For the walkthrough!)
* rainwarrior (For originally writing `fceux_symbols.py`)