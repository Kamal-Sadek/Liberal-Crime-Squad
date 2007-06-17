================================================================================
                      Liberal Crime Squad Linux Notes
================================================================================
2004-08-17 - Kevin Sadler (sadler@geodude.eclipse.co.uk)

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






