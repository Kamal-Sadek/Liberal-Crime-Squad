================================================================================
                      Liberal Crime Squad Linux Notes
================================================================================
2004-08-17 - Kevin Sadler (sadler@geodude.eclipse.co.uk)
2009-02-22 - Jonathan Stickles (jonathansfox@gmail.com)
2011-07-26 - Christian Sergelius (cribozai@member.fsf.org)
2013-09-13 - Rich McGrew (mcgrew_rich@yahoo.com)

Updated notes for compiling and playing under Linux for 4.xx.x versions
=======================================================================

Thanks to the Bay12Games forums user named xczxc who rightly pointed out the
need to update this document.

To compile LCS under Linux you will need to obtain the tools for building
under your specific distribution. You will also need to development libraries
for ncurses. The ncursesw version with wide character support should also
work.

To do this under Ubuntu (and most likely Debian too) issue the following
commands:

sudo apt-get install build-essential autoconf libncurses5-dev

The actual compilation will be done by changing your working directory to the
lcs folder and issuing:

chmod 744 bootstrap
./bootstrap
./configure
make

If the compilation was successful no errors should be issued but you will
see a myriad of warnings displayed all of which are safe to ignore.

At the moment due to continuous development of the game it is probaby
safer to not install the game systemwide and thus the best option is to
play it from the games own directory that resides in your home folder.

To do this with little trouble issue the following command while in
the LCS game folder:

cp src/crimesquad .

To run the game just type:

./crimesquad

To make everything display properly you should resize your terminal to 80x25
because the default of 80x24 sometimes makes the last lines information
appear in the wrong place.

Other good to know information
==============================

The savefile and highscores are independent of the game folder and reside
in a hidden folder on your home are with the name .lcs and you can navigate
there using your newly aquired terminal skills with the cd command as follows:

cd ~/.lcs

Build notes for Mac OS X
========================
(written by shibby1295 at bay12forums.com, Sep. 2013)

I just got the latest version of LCS running on my Macbook Pro. It took just a
smidgeon of doing, so I thought I'd document what I did. It's mostly the same
as building on Linux, with just a few changes.

This is on OSX 10.84, 64 bit intel. Your mileage may vary. Also, I haven't
played very much yet, so there could be bugs hiding in specific gameplay areas.

To get SVN:
  Install XCode via the App Store
  Open XCode, then go to the Downloads tab of the preferences menu
  Click the 'install' button next to "Command Line Tools"
To get autoconf: (in terminal):
  ruby -e "$(curl -fsSL https://raw.github.com/mxcl/homebrew/go)"
  brew install automake
To build LCS:
  svn co http://svn.code.sf.net/p/lcsgame/code/trunk lcsgame
  cd lcsgame/
  ./bootstrap
  LIBS="/usr/lib/libiconv.2.dylib" ./configure
  make
To play:
  src/crimesquad

Troubleshooting
===============

My game does not compile properly, what is wrong?!

The problem can be one of many and depends on too many things to give
a simple answer. You can get help by contacting me (email address in
the beginning of this document). Just copy the last screen of output
that you were given and I will see what I can figure out.

---------------------------------------------------------------

My games runs smoothly and everything worked out fine untill I wanted to
load a saved game. All I got was a screen with errors and my terminal
seems messed up somehow. What gives?

This is a problem that is distribution and compiler version dependant.
The reason is quite complicated and as of yet I have had no time nor
want to figure out exactly what the problem is. But luckily there is
an easy fix that does not make you lose the saved game!

While in the game folder issue the following:

make clean
./configure CFLAGS=-g CXXFLAGS=-g
make
# Wait for it to compile, check your email and take a sip of your beverage
cp src/crimesquad .

Then run the game again. Hope it worked for you. 


Important Update as of Version 3.18
===================================

This file hasn't been updated in a long time, so I'll try to give a few
key notes that will update the information contained below.

BUILDING THE GAME
If you have this file, it's probably because you checked out the source
code from SVN. The new build commands are:

  bootstrap
  ./configure
  make

GRAPHICS AND TERMINAL SIZE
The graphics for the Linux version of LCS are unaltered since 3.09. I have
endeavored to ensure that future updates of LCS use strictly portable ASCII
so that futher problems are not created for the port. The note below about
terminal size remains important.

FLAG BURNING
The flag burning bug was fixed, and you can now safely burn flags in Linux.

AUTOSAVING
The game still autosaves, but instead of autosaving to autosave.dat, it
autosaves directly to save.dat.

ADDITIONAL REQUIRED FILES
There are several new files that you must keep accessible to the LCS
executable. These are found in the /workspaces directory that you probably
checked out from SVN alongside this file. The list is below:

  sitemaps.txt


Linux Notes for Version 3.09
============================

New features in this version (Windows and Linux):

Autosave - The game is automatically saved each turn (after wait is pressed)
           to a file called autosave.dat. If crimesquad crashes you can
	   rename this to save.dat to recover (you may want to rename the
	   original save.dat in case the autosave.dat contains something
	   which causes a repeatable crash, so you can revert to that one.
	   (but send Tarn Adams and me the bad save file so we can work on
	   the bug!)). See below for details on Save Files.
	   
Cursor Keys	Use the cursor keys to move your squad. Once you've been
		playing for a bit the WADX keys become a bit of a habit,
		but the cursor keys should make it easier for new players.

New Features for Linux:

Curses Graphics	The DOS CP437 graphics characters did not display correctly
		on Linux so a fix has been worked on (3.08 works if you
		install a font as described below). An "ASCII Hack" is
		now used where either the source code specifies a character
		by name rather than value or where a CP437 value gets converted
		to the native value. The conversion isn't perfect as there
		are a lot of characters that are not available (half blocks
		for example) and so are represented by a character that looks
		vaguely like it - you can tell what it's supposed to be now...
		In the future we can use Unicode/UTF-8 and we should be able
		to use exactly the same symbols. See the "Fonts" section 
		below.

Bugs for Linux:

Flag Burn	If you burn a flag under Linux the game will hang.

Terminal Size	Not really a bug but... the LCS screen size is 80x25
		the size of a DOS window, your default xterm window
		may be 80x24, the size of a VT100 screen, and so this
		may result in the last line of the display being misplaced.
		Simply increase the size of the terminal by resizing it
		by one or more lines (With the KDE Konsole from the
		menu select Setting > Size > 80 x 25 (IBM PC) to set the
		right size).

		
		
LCS Linux History
=================

Version 3.08 of Liberal Crime Squad (LCS) was ported to Linux and compiled 
with the GNU g++ compiler (GCC 3.3.2) and linked with the ncurses library 
(rather than the pdcurses library used on the Windows version).


LCS on UNIX
===========

It is likely that this code will compile on other versions of UNIX, but this
has not been verified yet. Provided you have a g++ compiler and ncurses you
should be in with a fighting chance. Read the header of the Makefile for some
clues for what to do.


LCS on Cygwin
=============

LCS compiles on Cygwin (a free Linux-like API/Environment for Windows 
http://www.cygwin.com ) with the Makefile supplied exactly as it does
for Linux, use PLATFORM=Linux in the Makefile. Cygwin will need to have
packages for development (eg. the gcc compiler and make) and the ncurses
library.


LCS on MinGW32
==============

MinGW32 is is a free Minimalist GNU compiler for Windows. 
( http://www.mingw.org ) It compiles with MinGW32 and the Msys
enviroment and PDcurses. There is a section for MinGW32 in the Makefile,
just uncomment it (and comment out the section for Linux above it). 


Build
=====

Unpack the tar archive into a suitable directory (e.g. /usr/local/src),
in the examples below assume nnn is the version number, eg 3.08:

  cd /usr/local/src
  tar -zxvf lcs_source_nnn.tgz 

  cd lcs_nnn
  
  make
  
This will compile the crimesquad executable. 


Installation
============

Currently the make file does not install LCS but it is very
simple.

LCS can be installed in two steps:

1. Copy the crimesquad executable to a suitable bin directory that is
   specified in PATH. If you have suitable permissions you may wish
   to copy it to /usr/local/bin, or to a bin directory in your home
   directory.

2. Install the art directory. The source directory has a subdirectory
   called art which contains some files used by the game. The game needs
   to know where these files are and will look in the following
   locations:
   
   /usr/share/lcs
   /usr/local/share/lcs
   
   To install the directory in /usr/local/share do the following:
   
   mkdir -p /usr/local/share/lcs
   cp -R art /usr/local/share/lcs/.
   
   Alternatively LCS will look for art in the current directory, or
   the environment variable LCS_ART_DIR may be set to the directory.
   An example of this using the bash shell:
   
   export LCS_ART_DIR=/usr/local/src/lcs_nnn/art
   
   Typically one would set this in .profile or .bashrc files.

3. Install the init.txt configuration file (optional, but lets you set
   additional configuration options). The game looks for this file in the
   current directory.
  
Fonts
=====

The first port, 3.08, output the DOS style CP437 codes for the curses graphics
which came out as accented characters and looked rather screwed up. The work
around for this was to install a CP437 font on UNIX as described below.

With 3.09 the "ASCII Hack" was developed which maps the CP437 codes onto
either curses alternative character set characters (the ACS_ macros) or
onto characters that (vaguely) look like what we need, it works reasonably
well.

The "perfect" solution is to use Unicode. This would require the use of
ncursesw (wide) which may need some code changes. The Unicode translation
is coded up but some more work is probably needed.

See cursesgraphics.h for more info.

By default the ASCII Hack is now used for Linux, if you have the CP437
fonts and like using them you could hack the code to define CH_USE_CP437
instead of CH_USE_ASCII_HACK.


CP437 Fonts
-----------

LCS uses DOS CodePage 437 characters for it's graphics (the flag, the map,
and the newspaper headlines). Linux doesn't support this as is, however
you can download some fonts from Emil Mikulic's website that you can use:

http://dmr.ath.cx/misc/cp437/

http://dmr.ath.cx/misc/cp437/fonts.tar.bz2

It is suggested that they be installed to /usr/share/fonts/cp437

The following line should be added to /etc/X11/XF86Config:

    FontPath   "/usr/share/fonts/cp437"

To run crimesquad in an xterm with these fonts type:

xterm -fn cp437-8x12 -e crimesquad


Why are the fonts different?
----------------------------

In simple terms the graphics characters binary codes lie above the portable
ASCII values and the character set that you are using on Linux renders
this as a different "glyph" to that which appears under DOS using CP437.
The fonts are just a hack - they change the glyphs under Linux to look
like those under DOS.

The "proper" way to fix this is to use unicode, or UTF-8, and ncursesw 
(ncurses for wide characters) and remap the CP437 values to their UTF-8
values. This will mean that LCS could be played on Linux in your favourite
font. The downside, apart from the porting required, is that not all Linux
boxes may be configured with UTF-8 support or have the ncursesw library
installed, though most of the latest distros should be able to cope.


Save Files
==========

The Windows crimesquad save files (save.dat) are binary compatible with
Linux on ix86 (don't expect them to work on PowerPC (eg. Macs, RISC) due
to endianess).

Under Linux/UNIX LCS creates a sub-directory in the user's home directory
called .lcs in which the save.dat file is stored. To transfer your file
to another user just create that directory:

	mkdir ~/.lcs

and copy the file into it.

With version 3.09 LCS creates an automatic save just before the "wait"
command is processed (before the days activities, such as squad raids
are carried out). This file is called autosave.dat and is stored in the
same place as the save.dat file (~/.lcs on UNIX or the game directory on
Windows). In the rare event that the game should crash ;) you may rename
the autosave.dat file to save.dat to recover, under UNIX:

	mv -f ~/.lcs/autosave.dat ~/.lcs/save.dat

SDL2 and SDL2_mixer
===================

Liberal Crime Squad can use SDL 2.0.3 and SDL_mixer 2.0 for audio.
The headers and libraries for those are included in the /src/sdl
directory. However the libraries to link against only work with
Microsoft Visual C++ (32-bit) and MinGW-GCC (32 bit), the 2 main
compilers used on MS Windows. On other platforms such as Linux and
Mac OS X you will probably need the SDL2 and SDL2_mixer libraries to
link against as dependencies, in order for it to compile correctly
with audio support. Currently this is used for MIDI background music.

So you have a couple alternatives:

1) Compile with SDL2 and SDL2_mixer. You will need a MIDI device.
Both Windows and Mac OS X have native MIDI that is supported by
SDL_mixer, along with support for Timidity. On Linux you have several
options for MIDI support: Timidity, which uses Gravis UltraSound
Patches (NOT Timidity++, which uses SoundFonts), is built-in, but
you'll need to put the Gravis UltraSound Patches in the location
specified by the SDL2_mixer documentation. That location is
Win32 in C:\timidity, and on UNIX in /usr/local/lib/timidity/.
There also might be native MIDI support on Linux but only with some
sound drivers and not others.

2) Use FluidSynth or JACK. Both of them are external programs but if
you install them, SDL2_mixer can use them for MIDI support.
FluidSynth is recommended if you want the best sound quality. It uses
SoundFonts. Personally I am fond of the Arachno SoundFont 1.0 which
you can find here: http://www.arachnosoft.com/main/soundfont.php. It
is designed to sound very nice with video games, but it takes up
148 megabytes. There are plenty of other great SoundFonts, including
many that take up less disk space, that one's just my personal
favorite, you might like a different one better.

I am not quite sure about how the MIDI support in JACK works. Maybe
it is just an alternative for ALSA, for the people on *BSD systems,
since SDL and SDL_mixer don't support the native OSS on *BSD. Anyway,
if you are on something like FreeBSD, OpenBSD, or NetBSD, you will
need JACK along with either the built-in Timidity or an external
FluidSynth. If you are on Linux rather than some other UNIX you have
ALSA so you don't need JACK, since SDL and SDL_mixer can use either one.
And as for Mac OS X/Darwin, it might be BSD-based but it has CoreAudio
and CoreMIDI, both supported natively by SDL and SDL_mixer. Well actually
CoreMIDI is just on iOS but SDL and SDL_mixer support both Android and
iOS.


3) If all else fails, uncomment the line that says #define DONT_INCLUDE_SDL
in common.h. This will disable SDL, SDL_mixer, and audio, and let
you compile and build Liberal Crime Squad without any SDL dependencies.
The only downside to this is you don't get any audio.
