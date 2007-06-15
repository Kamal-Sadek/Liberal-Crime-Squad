#include <iostream>
#include "lcsio.h"
#ifdef WIN32
#include <windows.h>
#include "curses.h"
#define CH_USE_CP437
#else
#include <ncurses.h>
#define CH_USE_ASCII_HACK
#endif
#include "cursesgraphics.h"

using namespace std;


int record[256];

int main(int nargs, char *args[])
{
	unsigned long picnum,dimx,dimy;
unsigned char bigletters[27][5][7][4];
unsigned char newstops[5][80][5][4];
unsigned char newspic[20][78][18][4];
int p, x, y;

	DWORD numbytes;
	HANDLE h;

	h=LCSCreateFile("art\\largecap.cpc", LCSIO_READ);

    initscr();
    
    for (p=0; p<256; p++)
    {
    	record[p]=0;
    }
	
	if(h==NULL)
	{
		//getch();
		
		exit(1);
	}
	else
		{
		ReadFile(h,&picnum,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&dimx,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&dimy,sizeof(unsigned long),&numbytes,NULL);
		
		//cout << "Number of Pictures: " << picnum << endl;
		//cout << "Size: x: " << dimx << " by y: " << dimy << endl;
		
		//First char is the character, the last 3 are for setcolor().
		
		for( p=0;p<picnum;p++)
			{
			for( x=0;x<dimx;x++)
				{
				for( y=0;y<dimy;y++)
					{
					ReadFile(h,&bigletters[p][x][y][0],sizeof(unsigned char)*4,&numbytes,NULL);
					record[bigletters[p][x][y][0]]++;
				    addch(translateGraphicsChar(bigletters[p][x][y][0]));
					}
				//cout << endl;
				    addch('\n');
				}
				//cout << endl;
        refresh();
        getch();
        clear();
			}
		CloseHandle(h);
		
		
				

	h=LCSCreateFile("art\\newstops.cpc", LCSIO_READ);
	if(h!=NULL)
		{
		ReadFile(h,&picnum,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&dimx,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&dimy,sizeof(unsigned long),&numbytes,NULL);
		for(int p=0;p<picnum;p++)
			{
			for(int x=0;x<dimx;x++)
				{
				for(int y=0;y<dimy;y++)
					{
					ReadFile(h,&newstops[p][x][y][0],sizeof(unsigned char)*4,&numbytes,NULL);
					record[newstops[p][x][y][0]]++;
					}
				}
			}
		CloseHandle(h);
		}

	h=LCSCreateFile("art\\newspic.cpc", LCSIO_READ);
	
	if(h!=NULL)
		{
		ReadFile(h,&picnum,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&dimx,sizeof(unsigned long),&numbytes,NULL);
		ReadFile(h,&dimy,sizeof(unsigned long),&numbytes,NULL);
		for(int p=0;p<picnum;p++)
			{
			for(int x=0;x<dimx;x++)
				{
				for(int y=0;y<dimy;y++)
					{
					ReadFile(h,&newspic[p][x][y][0],sizeof(unsigned char)*4,&numbytes,NULL);
					record[newspic[p][x][y][0]]++;
					}
				}
			}
		CloseHandle(h);
		}
		
	//	for(p=0;p<256;p++)
	//	{
	//		cout << "code " << p << " = " << record[p] << endl;
	//	}
		
	endwin();	
	return 0;	
}
}


