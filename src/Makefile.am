bin_PROGRAMS = crimesquad
crimesquad_SOURCES = game.cpp lcsio.cpp cursesmovie.cpp compat.cpp\
 cursesgraphics.cpp lcsio.h cursesmovie.h compat.h cursesgraphics.h includes.h\
 externs.h common/commondisplay.cpp common/commonactions.cpp\
 common/consolesupport.cpp common/getnames.cpp common/translateid.cpp\
 common/equipment.cpp creature/augmentation.cpp creature/creature.cpp title/titlescreen.cpp\
 title/highscore.cpp title/newgame.cpp title/saveload.cpp basemode/basemode.cpp\
 basemode/baseactions.cpp basemode/activate.cpp basemode/reviewmode.cpp\
 sitemode/advance.cpp sitemode/mapspecials.cpp sitemode/newencounter.cpp\
 sitemode/sitemode.cpp sitemode/talk.cpp sitemode/miscactions.cpp\
 sitemode/sitedisplay.cpp sitemode/stealth.cpp combat/chase.cpp\
 combat/fight.cpp combat/haulkidnap.cpp daily/activities.cpp daily/daily.cpp\
 daily/date.cpp news/news.cpp daily/shopsnstuff.cpp daily/siege.cpp\
 monthly/justice.h      monthly/justice.cpp \
 monthly/monthly.h      monthly/monthly.cpp monthly/lcsmonthly.cpp \
 politics/amendments.h  politics/amendments.cpp \
 politics/law.h         politics/law.cpp \
 politics/politics.h    politics/politics.cpp \
 monthly/sleepers.h     monthly/sleepers.cpp \
 daily/interrogation.cpp\
 daily/recruit.cpp politics/alignment.h \
 configfile.cpp configfile.h news/ads.cpp news/headline.cpp news/layout.cpp\
 news/majorevent.cpp news/squadstory_text.cpp sitemode/sitemap.h\
 sitemode/sitemap.cpp news/news.h\
 locationdef.h basemode/activate_sleepers.cpp \
 items/armor.cpp creature/augmenttype.cpp creature/creaturetypes.cpp basemode/liberalagenda.cpp\
 common/misc.cpp common/help.cpp creature/augmentation.h creature/creature.h\
 creature/creaturenames.cpp cmarkup/Markup.cpp cmarkup/Markup.h vehicle/vehicle.cpp vehicle/vehicle.h\
 vehicle/vehicletype.cpp vehicle/vehicletype.h items/weapontype.h\
 items/armortype.cpp items/armortype.h items/clip.cpp items/clip.h\
 items/cliptype.cpp items/cliptype.h items/item.cpp items/item.h\
 items/itemtype.cpp items/itemtype.h items/loot.cpp items/loot.h\
 items/loottype.cpp items/loottype.h items/money.cpp items/money.h\
 items/weapon.cpp items/weapon.h items/weapontype.cpp items/armor.h\
 sitemode/shop.h sitemode/shop.cpp log/log.h log/log.cpp locations/locations.cpp\
 title/initfile.cpp locations/world.cpp locations/locations.h common.h creature/augmenttype.h\
 creature/creaturetype.h creature/creaturetype.cpp common/stringconversion.cpp

crimesquad_CPPFLAGS = -DINSTALL_DATA_DIR=\"$(datadir)\" \
		      -I$(top_srcdir)/lib/libsdl2 \
		      -I$(top_srcdir)/lib/pdcurses

crimesquad_LDADD = ${top_builddir}/lib/pdcurses/libpdcurses.la
if BUILD_WIN32
crimesquad_LDFLAGS = -static -static-libgcc -static-libstdc++
endif

