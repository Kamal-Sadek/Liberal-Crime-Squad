Support Libraries
=================

These are libraries gleaned from elsewhere and required for building LCS itself.

libSDL2
-------

This is a pure upstream copy of libSDL2 downloaded directly from
[libSDL.org](http://libsdl.org).  It started as release 2.0.7.

libSDL2 is licensed under the zlib license.

To update to a new release, should it prove necessary, download the new tarball,
then cd into the libsdl2 source directory, unpack it, and check it in.
```
 cd lib/libsdl2
 tar --extract --strip-components=1 --gzip --file ~/Downloads/SDL2-2.0.7.tar.gz
 git add -A
 git commit -m "Update libSDL2 to 2.0.7"
 git tag import-libsdl-2.0.7
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

tinyxml2
--------

This is a pure upstream copy of TinyXML-2 downloaded directly from
[GitHub](https://github.com/leethomason/tinyxml2).  It started as release 5.0.1.

TinyXML-2 is licensed under the zlib license.

To update to a new release, should it prove necessary, download the new tarball,
then cd into the libsdl2 source directory, unpack it, and check it in.
```
 cd lib/tinyxml2
 tar --extract --strip-components=1 --gzip --file ~/Downloads/timyxml2-5.0.2.tar.gz
 cp timyxml2.h tinyxml2.cpp ../../src
 git add -A
 git commit -m "Update TinyXML-2 to 5.0.2"
 git tag import-tinyxml2-5.0.2
```

TinyXML-2 actually consists of a single header and source file:  it's easier to
just copy those two files into the `src` directory and build them there than to
enrobe the entire soure distribution in out build system.  It is provided here
for provenance.

