////////////////////////////////////////////////////////////////////////////////
//																				
//Copyright (c) 2004 by Kevin Sadler						
//																				
////////////////////////////////////////////////////////////////////////////////
//This file is part of Liberal Crime Squad.					
//																				
//    Liberal Crime Squad is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//														
//    Liberal Crime Squad is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.										
//																						
//    You should have received a copy of the GNU General Public License
//    along with Liberal Crime Squad; if not, write to the Free Software	
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////

/* These functions mimic the Windows API but pass through to POSIX IO functions.*/

#include "lcsio.h"
#include <stdlib.h>
#include <string.h>

/* 
 * For UNIX platforms we will create a .lcs directory in the user's home
 * directory which will contain the save file and high scores.
 * lcs_home will store this value.
 * 
 * The art directory may be stored on the system. It may be in a "sensible"
 * place, a place defined by the environment variable LCS_ART_DIR or in the
 * current directory.
 */

#ifndef WIN32
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
  //User's LCS home directory for save files
  bool lcs_home_init = false;
  char *lcs_home = NULL;
  int len_home = 0;
  //The LCS art data location
  bool lcs_art_init = false;
  char *lcs_art = NULL;
  int len_art = 0;

  const char *possible_art_dir[] =
  { 
	"/usr/share/lcs/art",
	"/usr/local/share/lcs/art",
	"./art",
  };
  #define NUM_POSSIBLE_ART_DIR 3

#endif

HANDLE LCSCreateFile(char *filename, enum LCSIOFlag flag)
{
HANDLE h;
#ifdef WIN32
	if(flag==LCSIO_READ)
		{
		h=CreateFile(filename,GENERIC_READ|GENERIC_WRITE,
					 0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		}
	else
		{
		h=CreateFile(filename,GENERIC_READ|GENERIC_WRITE,
					 0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		}

	if(h==INVALID_HANDLE_VALUE)
	{
	h = NULL;
	}
#else
char *p = NULL;
char *p1 = NULL;
char *f = NULL;
int tofree = 0;
int n = 0;

static const char *fopen_write = "w+b";
static const char *fopen_read = "r+b";
const char *fopen_flag = fopen_read;
char *home = NULL;
char *art = NULL;
struct stat buf;

if (flag == LCSIO_WRITE)
{
	fopen_flag = fopen_write;
}

if (! lcs_home_init)
  {
  	home = getenv("HOME");
  	
  	len_home = strlen(home) + 1 + 4 + 1;
  	
  	lcs_home = (char *)malloc(len_home * sizeof(char));
  	
  	sprintf(lcs_home, "%s/.lcs", home);
  	
  	if (stat(lcs_home, &buf) != 0)
  	{
  		if (mkdir(lcs_home, 0750) != 0)
  		{
  			//error unable to create .lcs directory.
  			//TODO Need to exit from here cleanly.
    	  	fprintf(stderr,"Error: Unable to create LCS home directory: %s\n",
    	  	lcs_home);
  			
  		}
  		else
  		{
  			lcs_home_init = true;
  		}
  	}	
  }

if (strncmp(filename, "art\\", 4) == 0)
{
	
if (! lcs_art_init)
  {
  	//First check for an environment variable...
  	art = getenv("LCS_ART_DIR");
 
    //If we got a dir check it exists, if it doesn't ignore it and look 
    // at the other possible directories. 	
  	if (art != NULL)
  	  {
       	if (stat(art, &buf) != 0)
       	{
       		//TODO Invalid LCS_ART_DIR warning
    	  	fprintf(stderr,"Warning: LCS art directory: %s\n",art);
    	  	fprintf(stderr,"         as defined by environment variable LCS_ART_DIR\n");
    	  	fprintf(stderr,"         could not be found. Checking for other possibilities...\n");
       		art = NULL;
       	}  	  	
  	  }
 
    if (art == NULL)
    {
    	for (n = 0; n < NUM_POSSIBLE_ART_DIR; n++)
    	{
        	if (stat(possible_art_dir[n], &buf) == 0)
  	          {
  	          	art = (char *)possible_art_dir[n];
  	          	break;
  	          }   		
    	}
    	//Hmmm, didn't find the art dir in any of the possible locations...
    	if (art == NULL)
    	  {
    	  	//TODO exit - can't find art
    	  	fprintf(stderr,"Error: Unable to find LCS art directory.\n");
    	  	exit(1);
    	  }
    	  else
    	  {
    	  	lcs_art = art;
    	  	len_art = strlen(art);
    	  	lcs_art_init = true;
    	  }
    }
  	else
  	{
    //We need to make a copy of the environment variable value.
  	len_art = strlen(art);  	
  	lcs_art = (char *)malloc((len_art + 1) * sizeof(char));
    lcs_art_init = true;
    strcpy(lcs_art,art);
    lcs_art[len_art]='\0';
  	}
  }	

//Skip the "art\" at the beginning...		
p1 = strchr(filename,'\\');

  if (p1 != NULL)
  {
   p1++;
   tofree = strlen(p1)+1+strlen(lcs_art);
   f = (char *)malloc((tofree+1)*sizeof(char));   
   sprintf(f, "%s/%s", lcs_art, p1);	
 
   p = strchr(f,'\\');
 
   while (p != NULL)
   {
   *p='/'; 
   p = strchr(p,'\\');
   }
  }
  else
  {
   f = filename;
  }
}
else
{
  tofree = len_home + 1+ strlen(filename);
  f = (char *)malloc((tofree+1)*sizeof(char));
  sprintf(f, "%s/%s", lcs_home, filename);	
}

	h = fopen(f,fopen_flag);

	if (tofree>0)
	{
	 free(f);
	}
#endif
return h;
}

#ifndef WIN32

void WriteFile(HANDLE h, void *data, size_t in, unsigned long *out, void *null)
{
		*out = (unsigned long)fwrite(data,in,1,h);
}
 
void ReadFile(HANDLE h, void *data, size_t in, unsigned long *out, void *null)
{
		*out = (unsigned long)fread(data,in,1,h);
}

void CloseHandle(HANDLE h)
{
 fclose(h);
}
 
#endif
