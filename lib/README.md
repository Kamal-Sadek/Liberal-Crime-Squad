Support Libraries
=================

These are libraries gleaned from elsewhere and required for building LCS itself.

libSDL2
-------

This is a pure upstream copy of libSDL2 downloaded directly from
[libSDL.org](http://libsdl.org).  It started as release 2.0.7.

To update to a new release, should it prove necessary, download the new tarball,
then cd into the libsdl2 source directory, unpack it, and check it in.
```
 cd lib/libsdl2
 tar --extract --strip-components=1 --gzip --file ~/Downloads/SDL2-2.0.7.tar.gz
 git add -A
 git commit -m "Update libSDL2 to 2.0.7"
 git tag import-libSDL-2.0.7
```

The source for libSDL2 is self-contained and operates as a subproject (in
autotools terms, an AC_CONFIG_SUBDIRS project.  Be aware that the downloaded
tarball ships with generated files and if you regenerate them, the project will
fail to build.

pdcurses
--------

This is a heavily bastardized version of PDCurses as obtained from Bill Gray's
excellent [Pluto fork](https://github.com/Bill-Gray/PDCurses) of the "official'
PDCurses project.  Unfortunately, that project does not really lend itself to
embedding in another project, so because it's "public domain" I grabbed the
relevant source files (the core library code and, after some experiementation with
native Win32 and X11 variants, the sdl2 code), wrote my own Makefile.am and
integrated it as a subproject under lcs.

To update these files, you'll just have to clone the upstream repo and copy the
source files out.
```
 cd lib/pdcurses
 cp /tmp/PDCurses/pdcurses/*.[ch] .
 cp /tmp/PDCurses/sdl2/*.[ch] sdl2
 git add -A
 git commit -m "Update pdcurses"
```

catch2
------

This is the most excellent Catch2 BDD unit testing franework for C++ from Phil
Nash.  It started as an import of release 2.0.1.

To update to a new release, should it prove necessary, download the new tarball,
then cd into the catch2 source directory, unpack it, copy out the relevant
header files. and check the whole mess in.
```
 cd lib/catch2
 tar --extract --strip-components=1 --gzip --file ~/Downloads/Catch2-2.0.2.tar.gz 
 cd ../..
 cp -v lib/catch2/single_include/catch.hpp tests/include
 cp -v lib/catch2/include/reporters/catch_reporter_tap.hpp tests/include
 git add -A
 git commit -m "Update Catch2 to 2.0.2"
 git tag import-catch2-2.0.2
```

