bin_PROGRAMS = crimesquad 
crimesquad_SOURCES = game.cpp lcsio.cpp cursesmovie.cpp compat.cpp cursesgraphics.cpp\
 lcsio.h cursesmovie.h compat.h cursesgraphics.h\
 common/commondisplay.cpp common/commonactions.cpp common/consolesupport.cpp\
 common/getnames.cpp common/translateid.cpp common/equipment.cpp common/creature.cpp\
 title/titlescreen.cpp title/highscore.cpp title/newgame.cpp title/saveload.cpp
crimesquad_CPPFLAGS = $(CPPFLAGS) -DINSTALL_DATA_DIR=\"$(datadir)\"
crimesquad_CXXFLAGS = $(CXXFLAGS) -I$(top_srcdir)/src
