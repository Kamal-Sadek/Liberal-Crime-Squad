#!/bin/sh

find . -name "*~" -type f | xargs rm -i
find . -name ".#*" -type f | xargs rm -i
find . -name "*.rej" -type f | xargs rm -i
find . -name "*.orig" -type f | xargs rm -i
find . -name ".DS_Store" -type f | xargs rm -i # <http://en.wikipedia.org/wiki/.DS_Store>
find . -name "thumbs.db" -type f | xargs rm -i # <http://en.wikipedia.org/wiki/Windows_thumbnail_cache>
find . -name ".*.swp" -type f | xargs rm -i
#find . -name "DEADJOE" -type f | xargs rm -i
#find . -type f | grep -v ".psp" | grep -v ".gif" | grep -v ".jpg" | grep -v ".png" | grep -v ".tgz" | grep -v ".ico" | grep -v "druplicon" | xargs perl -wi -pe 's/\s+$/\n/'
#find . -type f | grep -v ".psp" | grep -v ".gif" | grep -v ".jpg" | grep -v ".png" | grep -v ".tgz" | grep -v ".ico" | grep -v "druplicon" | xargs perl -wi -pe 's/\t/  /g'
