/*

This file is a complete rewrite of the LCS I/O system.

The original lcsio.h can be found in lcsio-old.h in the sourceforge Subversion
repository.

This file is part of Liberal Crime Squad.

    Liberal Crime Squad is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Liberal Crime Squad is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Liberal Crime Squad; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   02111-1307   USA
*/

/**
    lcsio.cpp

    Chris Johnson

    Input/Output for Liberal Crime Squad.

    This file contains all of the declarations for the new Liberal Crime Squad I/O system.
    The entire system is being rewritten
*/


#ifndef LCSIO_H_INCLUDED
#define LCSIO_H_INCLUDED

#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <direct.h>
#else
#include <stdio.h>


#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
/**
    I/O Flags for the LCS I/O System.


*/
enum LCSIO_FLAGS
{
    LCSIO_PRE_ART = 1, /// Append the given file name to the art directory path.
    LCSIO_PRE_HOME = 2, /// Append the given file name to the home directory path.
    //LCSIO_PRE_CONFIG = 3 /// Append the given file name to the config directory path.
};


/**
    LCSIO File open.

    This function will open the given file using
    a prefix specified in flags. Use the mode
    just as you would for fopen().

    Use the prefix flags to access files in platform-specific locations.
    Prefix art will give you <lcsartdir>/<filename>.
    Prefix home will give you <lcshomedir>/<filename>.
        Home defaults to '~/.lcs/' on *nix, '.' on Windows.
    Never mix prefixes, behavior is undefined in that case and may
    cause files to be deleted or corrupted.

    filename Name of the file to open.
    mode Mode of stream. Same as mode for fopen().
    flags File name prefix flags, see enum LCSIO_FLAGS
    FILE* handle. NULL on error.

*/
FILE* LCSOpenFile(char* filename,char* mode,int flags);

/**
    LCSIO Close an open file.

    This function closes the given file handle.
    handle File handle to close.

*/
void LCSCloseFile(FILE* handle);




/**
    Write to file.

    This function simply passes through to the standard C I/O system,
    but returns false on error, and true on success.
    If you need something more, use the actual C I/O calls instead.

    buffer Pointer to buffer to write.
    size Size of data to write, normally 1 byte.
    count Number of data items to write. This is the length of the data.
    handle FILE* handle that you get from LCSOpenFile.
    False on error, true if no error.
*/
/*inline bool LCSWrite(void* buffer, size_t size, size_t count, FILE* handle)
{
    fwrite(buffer,size,count,handle);
    if(ferror(handle))
        return false;

    else
        return true;
} Deprecated function - SAV */


/**
    Read from file to buffer.

    This function simply passes through to the standard C I/O system,
    but returns false on error and true on success.
    If you need something more, use the actual C I/O calls instead.

    buffer Pointer to buffer to read in to.
    size Size of data to read, normally 1 byte.
    count Number of data items to read. This is the length of the data.
    handle FILE* handle that you get from LCSOpenFile.
     False on error, true if no error.
*/
/*inline bool LCSRead(void* buffer, size_t size, size_t count, FILE* handle)
{
    fread(buffer,size,count,handle);
    if(ferror(handle))
        return false;

    else
        return true;
}Deprecated function - SAV */

/**
    Deletes a file.

    This function unlinks the filename you spefify.
    You may use prefix flags to delete files in other directories.

    Do not try to delete files in the art directory.

    filename Name of file to delete.
    flags Prefix flags. See enum LCSIO_FLAGS.
*/
void LCSDeleteFile(char* filename,int flags);


#endif // LCSIO_H_INCLUDED
