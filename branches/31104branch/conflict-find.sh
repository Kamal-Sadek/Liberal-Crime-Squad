#!/bin/sh
##
################################################################################
## A simple script that finds Subversion(1) conflicts using grep(1), and      ##
##        displays them using sed(1).                                         ##
##               This script does *NOT* fix them. *That* would be your job.   ##
################################################################################
##               FIXME: Poorly implemented, grep(1)s the directories 3 times. ##
################################################################################
args='--color=auto --exclude-dir=".svn"'

                                                       #####################
fcount=`grep -rlI $args \>\>\>\>\>\>\> * | sed -n $=`  ## grep(1), sed(1) ##
ccount=`grep -rI $args \>\>\>\>\>\>\> * | sed -n $=`   ## grep(1), sed(1) ##
                                                       #####################

if [ "$fcount" = "" ]; then
	fcount='any';
	fnum='file';
	elif [ "$fcount" = "1" ]; then
		fnum='file';
	else fnum='files';
fi

if [ "$ccount" = "" ]; then
	ccount='No';
	cnum='matches';
	elif [ "$ccount" = "1" ]; then
		cnum='match';
	else cnum='matches';
fi

                                                                   #############
grep -rnI $args \>\>\>\>\>\>\> *                                   ## grep(1) ##
echo "$ccount $cnum in $fcount $fnum." #########################################
                                       ## Recursive, grep(1) every file       ##
                                       ##  (except for the `.svn' directory)  ##
                                       ##  in every directory for a conflict  ##
                                       ##  indicator                          ##
                                       #########################################
