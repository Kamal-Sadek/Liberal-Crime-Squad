
include_HEADERS = \
  acs_defs.h curses.h curspriv.h panel.h term.h

noinst_LTLIBRARIES = libpdcurses.la

libpdcurses_la_SOURCES = \
  ${include_HEADERS} \
  addch.c addchstr.c addstr.c attr.c beep.c bkgd.c border.c \
  clear.c color.c debug.c delch.c deleteln.c deprec.c getch.c \
  getstr.c getyx.c inch.c inchstr.c initscr.c inopts.c insch.c \
  insstr.c instr.c kernel.c keyname.c mouse.c move.c outopts.c \
  overlay.c pad.c panel.c printw.c refresh.c scanw.c scr_dump.c \
  scroll.c slk.c termattr.c terminfo.c touch.c util.c window.c 

libpdcurses_la_SOURCES += \
  sdl2/pdcutil.c sdl2/pdcclip.c sdl2/pdcsdl.h sdl2/deffont.h sdl2/pdckbd.c \
  sdl2/pdcdisp.c sdl2/pdcsetsc.c sdl2/pdcscrn.c sdl2/pdcgetsc.c sdl2/sdltest.c \
  sdl2/deficon.h
  
libpdcurses_la_CPPFLAGS = -I${top_srcdir}/lib/libsdl2/include
libpdcurses_la_LIBADD = ${top_builddir}/lib/libsdl2/build/libSDL2.la

