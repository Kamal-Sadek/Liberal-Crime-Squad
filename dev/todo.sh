#!/bin/sh

args='--color=auto --exclude-dir=".svn"'
gstring='(todo|fixme)'
grep -inrIP $args "$gstring" * # grep(1)
