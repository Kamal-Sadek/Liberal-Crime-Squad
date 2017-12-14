#
# This file is part of Liberal Crime Squad.
#
# Liberal Crime Squad is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation, either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

bin_PROGRAMS = crimesquad
noinst_LTLIBRARIES = libcrimesquad.la

crimesquad_SOURCES = \
 game.cpp 

libcrimesquad_la_SOURCES = \
 basemode/baseactions.cpp basemode/activate.cpp basemode/reviewmode.cpp\
 basemode/liberalagenda.cpp\
 combat/fight.cpp combat/haulkidnap.cpp daily/activities.cpp daily/daily.cpp\
 common.h creature/augmenttype.h\
 common/commondisplay.cpp common/commonactions.cpp\
 common/consolesupport.cpp common/getnames.cpp common/translateid.cpp\
 common/equipment.cpp creature/augmentation.cpp \
 common/interval.h             common/interval.cpp \
 common/misc.cpp common/help.cpp creature/augmentation.h \
 common/stringconversion.cpp \
 configfile.h                  configfile.cpp \
 creature/attributes.h         creature/attributes.cpp \
 creature/creature.h           creature/creature.cpp \
 creature/creaturenames.cpp    creature/creaturetypes.cpp \
 creature/creaturetypecache.h  creature/creaturetypecache.cpp \
 creature/creaturetype.h       creature/creaturetype.cpp \
 creature/skill.h              creature/skill.cpp \
 cursesgraphics.cpp lcsio.h cursesmovie.h compat.h cursesgraphics.h includes.h\
 daily/date.h                  daily/date.cpp \
 daily/interrogation.cpp\
 daily/recruit.h               daily/recruit.cpp \
 daily/shopsnstuff.cpp \
 daily/siege.h                 daily/siege.cpp\
 items/armor.cpp creature/augmenttype.cpp \
 items/armortype.cpp items/armortype.h items/clip.cpp items/clip.h\
 items/cliptype.cpp items/cliptype.h items/item.cpp items/item.h\
 items/itemtype.cpp items/itemtype.h items/loot.cpp items/loot.h\
 items/loottype.cpp items/loottype.h items/money.cpp items/money.h\
 items/weapon.cpp items/weapon.h items/weapontype.cpp items/armor.h\
 lcsio.cpp cursesmovie.cpp compat.cpp\
 locationdef.h basemode/activate_sleepers.cpp \
 monthly/justice.h             monthly/justice.cpp \
 monthly/monthly.h             monthly/monthly.cpp monthly/lcsmonthly.cpp \
 monthly/sleepers.h            monthly/sleepers.cpp \
 news/ads.cpp news/headline.cpp news/layout.cpp\
 news/majorevent.cpp news/squadstory_text.cpp \
 news/news.h                   news/news.cpp \
 politics/alignment.h          politics/alignment.cpp \
 politics/amendments.h         politics/amendments.cpp \
 politics/law.h                politics/law.cpp \
 politics/politics.h           politics/politics.cpp \
 sitemode/advance.cpp sitemode/mapspecials.cpp sitemode/newencounter.cpp\
 sitemode/shop.h sitemode/shop.cpp log/log.h log/log.cpp locations/locations.cpp\
 sitemode/sitedisplay.cpp sitemode/stealth.cpp combat/chase.cpp\
 sitemode/sitemap.h            sitemode/sitemap.cpp \
 sitemode/sitemode.cpp sitemode/talk.cpp sitemode/miscactions.cpp\
 tinyxml2.h                    tinyxml2.cpp \
 title/highscore.cpp title/newgame.cpp title/saveload.cpp basemode/basemode.cpp\
 title/titlescreen.cpp\
 title/initfile.cpp locations/world.cpp locations/locations.h \
 vehicle/vehicle.h             vehicle/vehicle.cpp \
 vehicle/vehicletype.cpp vehicle/vehicletype.h items/weapontype.h\
 externs.h              globals.cpp \
 tinydir.h

crimesquad_CPPFLAGS = \
  -DINSTALL_DATA_DIR=\"$(datadir)\" \
  -I$(top_srcdir)/lib/libsdl2 \
  -I$(top_srcdir)/lib/pdcurses

libcrimesquad_la_CPPFLAGS = $(crimesquad_CPPFLAGS)
libcrimesquad_la_LIBADD = ${top_builddir}/lib/pdcurses/libpdcurses.la

crimesquad_LDADD = libcrimesquad.la
crimesquad_LDFLAGS = -static -static-libgcc -static-libstdc++
if BUILD_WIN32
crimesquad_LDFLAGS += -mwindows
endif

