# BPM - Counter for UNIX (c) by Marius Heuler 

[![asciicast](https://asciinema.org/a/414554.svg)](https://asciinema.org/a/414554)

This program can be distributed, if it's not modified in any kind.  

The purpose if this program ist to calculate the bpm (beats  
per minute) rate of a song. The bpm rate is the *"speed"* of  
a song and is needed for DJing. With this program you can  
easily calculate bpm by pressing a key for each beat.  
The program should run on all SYS V compatible unix systems.  
It was developed under Linux and tested with Solaris and OSF/1.  

## Compilation:

```
$ gcc bpm.c -o bpm -lcurses 
```

or 

```
$ cc  bpm.c -o bpm -lcurses
```

If you don't have `curses` library installed you can use  
`ncurses` insteed. Then you must use `-lncurses` insteed of   
`-lcurses` and probably change the include `<curses.h>`  
to something like `<ncurses/curses.h>` or `<ncurses.h>`.  

If you don't have either `curses` or `ncurses` you can use  
`#undef USE_CURSES` insteed of `#define USE_CURSES`. But this  
is not recommended, because it will only work in xterms or  
on terminals which ar vt100 compatible. Also the system must   
have termcap installed (on some BSD system you must use  
`-ltermcap` by compilation).  
 
If you get an error about `CLK_TCK` ist undefined, you must it  
by hand with `#define CLK_TCK` ticks. For ticks you must use  
the number of ticks per second, which is mentioned in the   
manual of the times function (man times) or somewhere else.  

**Marius Heuler**

## Send suggestions, problems or ... to:

**Email:**    heuler@informatik.uni-wuerzburg.de
           or heuler@cip.informatik.uni-wuerzburg.de

~~**WWW:**     http://www-info3.informatik.uni-wuerzburg.de./~heuler~~

