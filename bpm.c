/*****************************************************************

   BPM - Counter for UNIX (c) by Marius Heuler

   This program can be distributed, if it's not modified in any kind.

   The purpose if this program ist to calculate the bpm (beats
   per minute) rate of a song. The bpm rate is the "speed" of
   a song and is needed for DJing. With this program you can
   easily calculate bpm by pressing a key for each beat.
   The program should run on all SYS V compatible unix systems.
   It was developed under Linux and tested with Solaris and OSF/1.

   Compilation:

      gcc bpm.c -o bpm -lcurses
   or cc  bpm.c -o bpm -lcurses

   If you don't have curses library installed you can use
   ncurses insteed. Then you must use -lncurses insteed of
   -lcurses and probably change the include <curses.h>
   to something like <ncurses/curses.h> or <ncurses.h>.

   If you don't have either curses or ncurses you can use
   #undef USE_CURSES insteed of #define USE_CURSES. But this
   is not recommended, because it will only work in xterms or
   on terminals which ar vt100 compatible. Also the system must
   have termcap installed (on some BSD system you must use
   -ltermcap by compilation).

   If you get an error about CLK_TCK ist undefined, you must it
   by hand with #define CLK_TCK ticks. For ticks you must use
   the number of ticks per second, which is mentioned in the
   manual of the times function (man times) or somewhere else.

   Marius Heuler

   Send suggestions, problems or ... to:

   Email:   heuler@informatik.uni-wuerzburg.de
         or heuler@cip.informatik.uni-wuerzburg.de

   WWW:     http://www-info3.informatik.uni-wuerzburg.de./~heuler


*****************************************************************/

#define USE_CURSES
#define CLK_TCK 100

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>

#ifdef USE_CURSES
#include <curses.h>
#else
#include <termios.h>
#define clear()       printf("\033[H\033[2J")
#define addstr(str)   printf(str)
#define move(x,y)     printf("\033[%d;%dH",x,y)
#define refresh()
#define getch()       getchar()
#endif

int count;
long tstart, tcurr;
float bpm, err;
int cc;
char scr[256];
int neu;
struct tms dummy;


char bigscr[] =
"  ###      #     #####   #####  #       #######  #####  #######  #####   #####  " \
" #   #    ##    #     #       # #    #  #       #     # #    #  #     # #     # " \
"#     #  # #          #       # #    #  #       #           #   #     # #     # " \
"#     #    #     #####   #####  #    #  ######  ######     #     #####   ###### " \
"#     #    #    #             # #######       # #     #   #     #     #       # " \
" #   #     #    #             #      #  #     # #     #   #     #     # #     # " \
"  ###    #####  #######  #####       #   #####   #####    #      #####   #####  ";


void bigwrite(x, y, c)
    int x, y; char c;
{
  char buf[10];
  int i;

  c -= '0';
  for (i=0; i<7; i++) {
    move(y+i,x);
    strncpy(buf, &bigscr[c*8+i*80], 8); buf[8] = 0;
    addstr(buf);
  }
}

void bigbpm()
{
  char buf[10];

  if (bpm<1) sprintf(buf, "%03.0f", 0.0);
  else       sprintf(buf, "%03.0f", bpm-0.5);
  bigwrite(10, 13, buf[0]);
  bigwrite(20, 13, buf[1]);
  bigwrite(30, 13, buf[2]);
}

void display()
{
  if (count) tcurr = times(&dummy); else tcurr = tstart;
  if (tcurr - tstart > 0) {
     bpm = (float)count * 60.0 * CLK_TCK / (float)(tcurr - tstart);
  } else {
     bpm = 0.0;
  }
  if (bpm > 0) {
     err = (60 * CLK_TCK / (float)(tcurr - tstart)) / bpm * 100;
     if (err > 99.99 ) err = 99.99;
  } else err = 0.0;
  sprintf(scr, "Time: %06.2fs        Beats: %04d",
     (float)(tcurr - tstart) / CLK_TCK, count);
  move(5,5);
  addstr(scr);
  sprintf(scr, "BPM:  %06.1f +- %05.2f%%", bpm, err);
  move(7,5);
  addstr(scr);
  move(10,5);
  addstr("+ . . . . . . . . . + . . . . . . . . .");
  move(10,(count % 20)*2+5);
  addstr("|");
}


//void main()
int main(void)
{
#ifdef USE_CURSES
    initscr();
    cbreak();
    noecho();
    nonl();
#else
    struct termios tty;
    tcgetattr(0, &tty);
    tty.c_lflag &= ~ICANON;
    tty.c_oflag &= ~ONLCR;
    tty.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &tty);
#endif

  clear();

  count   = 0;
  neu     = 1;
  tstart  = times(&dummy);
  move(1,2);
  addstr("BPM - Counter for UNIX (c) by Marius Heuler V1.03");
  move(22,5);
  addstr("q = Quit   Key = Beat   RETURN = Reset");
  while(1)
  {
    display();
    bigbpm();
    refresh();
    cc = getch();
    if (cc == 'q' || cc == ';') {
       move(14,1);
#ifdef USE_CURSES
         refresh();
         endwin();
#else
       tty.c_lflag |= ICANON;
       tty.c_oflag |= ONLCR;
       tty.c_lflag |= ECHO;
       tcsetattr(0, TCSANOW, &tty);
#endif
       exit(0);
    }
    if (neu) {
       tstart = times(&dummy);
       neu    = 0;
       if ((cc != 13) && (cc != 10)) continue;
    }
    if ((cc == 13) || (cc == 10)) {
       count = 0;
       neu   = 1;
       continue;
    }
    count++;
  }
}
