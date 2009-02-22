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
#include <string>
#include <queue>
#include "lcsio.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#define MAX_PATH_SIZE 2048
char homedir[MAX_PATH_SIZE];
char artdir[MAX_PATH_SIZE];
char* arttest="newspic.cpc";
bool initialized=false;
char *art_search_paths[]=
{
    #ifdef INSTALL_DATA_DIR
    INSTALL_DATA_DIR "/lcs/art/",
    #endif
    #ifndef WIN32
    "/usr/local/share/lcs/art/",
    "/usr/share/lcs/art/",
    "/usr/games/share/lcs/art/",
    "/usr/games/lcs/art/",
    #endif
    "./art/",
    "../art/",

    NULL
};




//Check if filename exists on the system.
//Filename is the full path to the file.
//This works on directories too, but only in Linux!
bool LCSFileExists(const char* filename)
{


    struct stat st;
    int ret=stat(filename,&st);

    if(ret==0)
        return true;
    else
        return false;
}

//Put the home directory prefix in homedir.
//Create the home directory if it does not exist.
bool LCSInitHomeDir()
{
    #ifndef WIN32
    char* homeenv=getenv("HOME");
    #else
    char* homeenv="./";
    #endif

    //Do everything using STL String, it is safer that way.
    std::string *str=new std::string(homeenv);
    if(str->at(str->length()-1)!='/')
        str->append("/");

    #ifndef WIN32
    str->append(".lcs/");
    #endif


    strncpy(homedir,str->c_str(),MAX_PATH_SIZE);
    delete str;
    if((!LCSFileExists(homedir)) && (strncmp(homedir,".",1)!=0))
    {
        #ifdef WIN32
        if(_mkdir(homedir)!=0)
            return false;
        #else
        if(mkdir(homedir,0750)!=0)
            return false;
        #endif
    }

    return true;
}

//Put the art directory prefix in artdir.
bool LCSInitArtDir()
{
    char* artprefix;
    artprefix=art_search_paths[0];
    std::string tester;
    for(int i=1;artprefix!=NULL;++i)
    {
        tester=artprefix;
        tester.append(arttest);
        if(LCSFileExists(tester.c_str()))
            break;

        artprefix=art_search_paths[i];

    }
    if(artprefix==NULL)
        return false;

    strncpy(artdir,artprefix,MAX_PATH_SIZE);
    return true;
}



FILE* LCSOpenFile(char* filename,char* mode,int flags)
{
    if(!initialized)
    {
        LCSInitHomeDir();
        LCSInitArtDir();
        initialized=true;
    }
    std::string filepath;
    if(flags & LCSIO_PRE_ART)
    {
        filepath=artdir;
    }
    else if(flags & LCSIO_PRE_HOME)
    {
        filepath=homedir;
    }

    filepath.append(filename);

    return fopen(filepath.c_str(),mode);
}

void LCSDeleteFile(char* filename,int flags)
{

    if(!initialized)
    {
        LCSInitHomeDir();
        LCSInitArtDir();
        initialized=true;
    }

    std::string str;

    if(flags & LCSIO_PRE_ART)
    {
        str.append(artdir);
    }
    else if(flags & LCSIO_PRE_HOME)
    {
        str.append(homedir);
    }

    str.append(filename);

    unlink(str.c_str());





}

void LCSCloseFile(FILE* handle)
{
    fclose(handle);
}

