/*
    File created by Rich McGrew.

    This file includes useful macros and functions for universal use, not just
    in .cpp files, but in .h files too. Having them in includes.h isn't good
    enough because then they are not accessible to header files that are
    included in includes.h. You can't have 2 header files both include each
    other. Thus this file was designed to be something any file could include
    and make full use of, with general-purpose functions and macros that help
    simplify common coding tasks.

    All code released under GNU GPL.
*/

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

/* Macro definition */
#ifndef MAX
   // maximum of 2 numbers
   #define MAX(a,b) (((a)<(b))?(b):(a))
#endif

#ifndef MIN
   // minimum of 2 numbers
   #define MIN(a,b) (((a)>(b))?(b):(a))
#endif

#ifndef ABS
   // absolute value of a number
   #define ABS(x) ((x)<0)?(-x):(x)
#endif

#ifndef DIFF
   // difference between 2 numbers (absolute value of subtracting one from the other)
   #define DIFF(x,y) ((x)<(y)?((y)-(x)):((x)-(y)))
#endif

/* Deletes and removes a specified pointer from a container. */
template <class Container> void delete_and_remove(Container& c,int i)
{
   if(i<=-1)return; // check for invalid index, and don't delete anything in this case
	delete c[i];
   c.erase(c.begin()+i);
}

/* Deletes and removes a specified pointer from 2 containers it's in. 
   c1 and c2 must not be the same container! */
template <class Container> void delete_and_remove(Container& c1,int i1,Container& c2,int i2)
{
   if(i1<=-1||i2<=-1)return; // check for invalid index, and don't delete anything in this case
   if(c1[i1]==c2[i2])
   { // it's the same pointer, so just delete it once, then remove it from both containers
      delete_and_remove(c1,i1);
      if(c1!=c2||i1!=i2)c2.erase(c2.begin()+i2); // remove it from 2nd location, unless 2nd location is identical to 1st location
   }
   else
   { // it's 2 different pointers, so delete them both and remove them from both containers
      delete_and_remove(c1,i1);
      delete_and_remove(c2,i2);
   }
}

/* Deletes and removes all pointers in a container. */
template <class Container> void delete_and_clear(Container& c)
{
   while(!c.empty())
   {
      delete c.back();
      c.pop_back();
   }
}

/* Deletes and removes all pointers that 2 containers have in common. */
template <class Container> void delete_and_clear(Container& c1,Container& c2)
{
   for(int i1=c1.size()-1;i1>=0;i1--)
      for(int i2=c2.size()-1;i2>=0;i2--)
         if(c1[i1]==c2[i2])delete_and_remove(c1,i1,c2,i2);
}

#endif
