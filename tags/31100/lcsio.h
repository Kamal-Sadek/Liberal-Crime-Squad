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

#include <iostream>

#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
typedef FILE *HANDLE; 
typedef unsigned long DWORD;
#endif

enum LCSIOFlag
{
 LCSIO_WRITE,
 LCSIO_READ,
 LCSIO_ART,
}; 

HANDLE LCSCreateFile(char *filename, enum LCSIOFlag flag);

#ifndef WIN32
void WriteFile(HANDLE h, void *data, size_t in, unsigned long *out, void *null);
void ReadFile(HANDLE h, void *data, size_t in, unsigned long *out, void *null);
void CloseHandle(HANDLE h);
#endif
