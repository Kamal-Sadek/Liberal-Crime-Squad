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

/**
* \file		cursesgraphics.h
*
* \brief	Portability Macros for CP437 Code Page Graphics Characters
*
* $Author: sadler $
* $Date: 2004/08/11 20:20:43 $
* \version	$Name: HEAD $ $Revision: 1.10 $
*
* $Id: cursesgraphics.h,v 1.10 2004/08/11 20:20:43 sadler Exp $
*
* <HR>
* \b Liberal Crime Squad
*
* 
* <HR>
*
* \par Abstract
* 
* These definitions aim to replace hard coded character values for ascii
* above 0x7f, which are not portable.
* CH_USE_CP437 is for use on windows which uses the DOS console characters.
* These are the original values that have been replaced with MACROs.
* CH_USE_ASCII_HACK is a dirty hack (well, it started out dirty and improved)
* to replace the extended characters with some from the normal range which 
* might give a similar appearance.
* This should be portable to anything! It attempts to use the alternative
* character set (ACS) macros from curses, which, if they don't have the
* right character, will fall back to an ASCII hack anyway.
* 
* CH_USE_UNICODE is for machines with unicode support. Curses will need to
* support unicode (maybe libncursesw?). But a different call may be needed...
* (Untested).
* 
* The gchar table is provided to convert raw CP437 codes stored in the movie
* files into ASCII_HACK or UNICODE values.
* 
* Note that all 256 codes have a name and a value for the CP437 and Unicode 
* tables. Not all CP437 codes are used. Only where codes are used have the 
* ASCII_HACK values been calculated.
* 
* For any value 0-255 there will be a line on each of the three tables and in the
* gchar table. If the value is not used then it will be commented out for all the
* #define lists. In the table unused codes convert to zero (rather than the
* possibility of an undefined macro for ASCII_HACK) and have a comment "Not Used".
* 
* If a character were to be "turned on" do the following:
* 
* 	1. Uncomment the macro for the code for CP437
*   2. Uncomment the macro for ASCII_HACK and provide an alternative character.
*   3. Uncomment the macro for the code for Unicode
*   4. For the entry in gchar do:
* 		a) Remove "0,"
* 		b) Uncomment the macro
* 		c) Delete the "Not Used" Comment.
* 
* Information for the cp437 to Unicode conversion came from the following
* Microsoft document:
* 
*<PRE> 
* #    Name:     cp437_DOSLatinUS to Unicode table
* #    Unicode version: 2.0
* #    Table version: 2.00
* #    Table format:  Format A
* #    Date:          04/24/96
*</PRE> 
*
* <HR>
*
* \par History
*
* \par
* $Log: cursesgraphics.h,v $
* Revision 1.10  2004/08/11 20:20:43  sadler
* Fixed portable includes for curses header file.
*
* Revision 1.9  2004/08/11 20:14:19  sadler
* Added portable includes for curses header file.
*
* Revision 1.8  2004/08/11 18:28:38  sadler
* New translateGraphicsChar() routine.
* Replace NULL with an inverse video space.
*
* Revision 1.7  2004/08/08 12:45:03  sadler
* *** empty log message ***
*
* Revision 1.6  2004/08/08 07:49:55  sadler
* *** empty log message ***
*
* Revision 1.5  2004/07/04 21:15:30  sadler
* *** empty log message ***
*
* Revision 1.4  2004/07/04 11:03:03  sadler
* no message
*
* Revision 1.3  2004/07/01 21:19:48  sadler
* Added CH_BOX_DRAWINGS_LIGHT_VERTICAL (179) as used in burnflag()
*
* Revision 1.2  2004/06/30 21:58:58  sadler
* Added SHADE characters that are used in news stories.
*
*
*/

/* Add characters to the lists in CP437 numerical order.
 * Entries are needed for CP437, ASCII_HACK and UNICODE.
 * Entry should be added to the graphicschar array.
 * Bump CH_MAXIMUM.
 */

#ifdef WIN32
  #include "curses.h"
#else      
  #ifdef XCURSES
    #define HAVE_PROTO 1
    #define CPLUSPLUS  1
    /* Try these PDCurses/Xcurses options later...
    #define FAST_VIDEO 
    #define REGISTERWINDOWS
    */
    #include <xcurses.h> //This is the X11 Port of PDCurses
  //undo PDCurses macros that break vector class
    #undef erase
    #undef clear
  #else
    #ifdef NCURSES
      #include <ncurses.h>
    #else     
      #include <curses.h>      
    #endif 
  #endif 
#endif  

#ifdef CH_USE_CP437

  #define CH_BULLET			                             7
  #define CH_WHITE_SUN_WITH_RAYS			            15
  #define CH_BLACK_RIGHT_POINTING_POINTER			    16
  #define CH_BLACK_DOWN_POINTING_TRIANGLE			    31

    #define CH_NULL 0x00
/*  #define CH_START_OF_HEADING 0x01 */
/*  #define CH_START_OF_TEXT 0x02 */
/*  #define CH_END_OF_TEXT 0x03 */
/*  #define CH_END_OF_TRANSMISSION 0x04 */
/*  #define CH_ENQUIRY 0x05 */
/*  #define CH_ACKNOWLEDGE 0x06 */
    #define CH_BELL 0x07
/*  #define CH_BACKSPACE 0x08 */
/*  #define CH_HORIZONTAL_TABULATION 0x09 */
/*  #define CH_LINE_FEED 0x0a */
/*  #define CH_VERTICAL_TABULATION 0x0b */
/*  #define CH_FORM_FEED 0x0c */
/*  #define CH_CARRIAGE_RETURN 0x0d */
/*  #define CH_SHIFT_OUT 0x0e */
    #define CH_SHIFT_IN 0x0f
    #define CH_DATA_LINK_ESCAPE 0x10
/*  #define CH_DEVICE_CONTROL_ONE 0x11 */
/*  #define CH_DEVICE_CONTROL_TWO 0x12 */
/*  #define CH_DEVICE_CONTROL_THREE 0x13 */
/*  #define CH_DEVICE_CONTROL_FOUR 0x14 */
/*  #define CH_NEGATIVE_ACKNOWLEDGE 0x15 */
/*  #define CH_SYNCHRONOUS_IDLE 0x16 */
/*  #define CH_END_OF_TRANSMISSION_BLOCK 0x17 */
/*  #define CH_CANCEL 0x18 */
/*  #define CH_END_OF_MEDIUM 0x19 */
/*  #define CH_SUBSTITUTE 0x1a */
/*  #define CH_ESCAPE 0x1b */
/*  #define CH_FILE_SEPARATOR 0x1c */
/*  #define CH_GROUP_SEPARATOR 0x1d */
/*  #define CH_RECORD_SEPARATOR 0x1e */
    #define CH_UNIT_SEPARATOR 0x1f
/*  #define CH_SPACE 0x20 */
/*  #define CH_EXCLAMATION_MARK 0x21 */
/*  #define CH_QUOTATION_MARK 0x22 */
/*  #define CH_NUMBER_SIGN 0x23 */
/*  #define CH_DOLLAR_SIGN 0x24 */
/*  #define CH_PERCENT_SIGN 0x25 */
/*  #define CH_AMPERSAND 0x26 */
/*  #define CH_APOSTROPHE 0x27 */
/*  #define CH_LEFT_PARENTHESIS 0x28 */
/*  #define CH_RIGHT_PARENTHESIS 0x29 */
/*  #define CH_ASTERISK 0x2a */
/*  #define CH_PLUS_SIGN 0x2b */
/*  #define CH_COMMA 0x2c */
/*  #define CH_HYPHEN_MINUS 0x2d */
/*  #define CH_FULL_STOP 0x2e */
/*  #define CH_SOLIDUS 0x2f */
/*  #define CH_DIGIT_ZERO 0x30 */
/*  #define CH_DIGIT_ONE 0x31 */
/*  #define CH_DIGIT_TWO 0x32 */
/*  #define CH_DIGIT_THREE 0x33 */
/*  #define CH_DIGIT_FOUR 0x34 */
/*  #define CH_DIGIT_FIVE 0x35 */
/*  #define CH_DIGIT_SIX 0x36 */
/*  #define CH_DIGIT_SEVEN 0x37 */
/*  #define CH_DIGIT_EIGHT 0x38 */
/*  #define CH_DIGIT_NINE 0x39 */
/*  #define CH_COLON 0x3a */
/*  #define CH_SEMICOLON 0x3b */
/*  #define CH_LESS_THAN_SIGN 0x3c */
/*  #define CH_EQUALS_SIGN 0x3d */
/*  #define CH_GREATER_THAN_SIGN 0x3e */
/*  #define CH_QUESTION_MARK 0x3f */
/*  #define CH_COMMERCIAL_AT 0x40 */
/*  #define CH_LATIN_CAPITAL_LETTER_A 0x41 */
/*  #define CH_LATIN_CAPITAL_LETTER_B 0x42 */
/*  #define CH_LATIN_CAPITAL_LETTER_C 0x43 */
/*  #define CH_LATIN_CAPITAL_LETTER_D 0x44 */
/*  #define CH_LATIN_CAPITAL_LETTER_E 0x45 */
/*  #define CH_LATIN_CAPITAL_LETTER_F 0x46 */
/*  #define CH_LATIN_CAPITAL_LETTER_G 0x47 */
/*  #define CH_LATIN_CAPITAL_LETTER_H 0x48 */
/*  #define CH_LATIN_CAPITAL_LETTER_I 0x49 */
/*  #define CH_LATIN_CAPITAL_LETTER_J 0x4a */
/*  #define CH_LATIN_CAPITAL_LETTER_K 0x4b */
/*  #define CH_LATIN_CAPITAL_LETTER_L 0x4c */
/*  #define CH_LATIN_CAPITAL_LETTER_M 0x4d */
/*  #define CH_LATIN_CAPITAL_LETTER_N 0x4e */
/*  #define CH_LATIN_CAPITAL_LETTER_O 0x4f */
/*  #define CH_LATIN_CAPITAL_LETTER_P 0x50 */
/*  #define CH_LATIN_CAPITAL_LETTER_Q 0x51 */
/*  #define CH_LATIN_CAPITAL_LETTER_R 0x52 */
/*  #define CH_LATIN_CAPITAL_LETTER_S 0x53 */
/*  #define CH_LATIN_CAPITAL_LETTER_T 0x54 */
/*  #define CH_LATIN_CAPITAL_LETTER_U 0x55 */
/*  #define CH_LATIN_CAPITAL_LETTER_V 0x56 */
/*  #define CH_LATIN_CAPITAL_LETTER_W 0x57 */
/*  #define CH_LATIN_CAPITAL_LETTER_X 0x58 */
/*  #define CH_LATIN_CAPITAL_LETTER_Y 0x59 */
/*  #define CH_LATIN_CAPITAL_LETTER_Z 0x5a */
/*  #define CH_LEFT_SQUARE_BRACKET 0x5b */
/*  #define CH_REVERSE_SOLIDUS 0x5c */
/*  #define CH_RIGHT_SQUARE_BRACKET 0x5d */
/*  #define CH_CIRCUMFLEX_ACCENT 0x5e */
/*  #define CH_LOW_LINE 0x5f */
/*  #define CH_GRAVE_ACCENT 0x60 */
/*  #define CH_LATIN_SMALL_LETTER_A 0x61 */
/*  #define CH_LATIN_SMALL_LETTER_B 0x62 */
/*  #define CH_LATIN_SMALL_LETTER_C 0x63 */
/*  #define CH_LATIN_SMALL_LETTER_D 0x64 */
/*  #define CH_LATIN_SMALL_LETTER_E 0x65 */
/*  #define CH_LATIN_SMALL_LETTER_F 0x66 */
/*  #define CH_LATIN_SMALL_LETTER_G 0x67 */
/*  #define CH_LATIN_SMALL_LETTER_H 0x68 */
/*  #define CH_LATIN_SMALL_LETTER_I 0x69 */
/*  #define CH_LATIN_SMALL_LETTER_J 0x6a */
/*  #define CH_LATIN_SMALL_LETTER_K 0x6b */
/*  #define CH_LATIN_SMALL_LETTER_L 0x6c */
/*  #define CH_LATIN_SMALL_LETTER_M 0x6d */
/*  #define CH_LATIN_SMALL_LETTER_N 0x6e */
/*  #define CH_LATIN_SMALL_LETTER_O 0x6f */
/*  #define CH_LATIN_SMALL_LETTER_P 0x70 */
/*  #define CH_LATIN_SMALL_LETTER_Q 0x71 */
/*  #define CH_LATIN_SMALL_LETTER_R 0x72 */
/*  #define CH_LATIN_SMALL_LETTER_S 0x73 */
/*  #define CH_LATIN_SMALL_LETTER_T 0x74 */
/*  #define CH_LATIN_SMALL_LETTER_U 0x75 */
/*  #define CH_LATIN_SMALL_LETTER_V 0x76 */
/*  #define CH_LATIN_SMALL_LETTER_W 0x77 */
/*  #define CH_LATIN_SMALL_LETTER_X 0x78 */
/*  #define CH_LATIN_SMALL_LETTER_Y 0x79 */
/*  #define CH_LATIN_SMALL_LETTER_Z 0x7a */
/*  #define CH_LEFT_CURLY_BRACKET 0x7b */
/*  #define CH_VERTICAL_LINE 0x7c */
/*  #define CH_RIGHT_CURLY_BRACKET 0x7d */
/*  #define CH_TILDE 0x7e */
/*  #define CH_DELETE 0x7f */
/*  #define CH_LATIN_CAPITAL_LETTER_C_WITH_CEDILLA 0x80 */
/*  #define CH_LATIN_SMALL_LETTER_U_WITH_DIAERESIS 0x81 */
/*  #define CH_LATIN_SMALL_LETTER_E_WITH_ACUTE 0x82 */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX 0x83 */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_DIAERESIS 0x84 */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_GRAVE 0x85 */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_RING_ABOVE 0x86 */
/*  #define CH_LATIN_SMALL_LETTER_C_WITH_CEDILLA 0x87 */
/*  #define CH_LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX 0x88 */
/*  #define CH_LATIN_SMALL_LETTER_E_WITH_DIAERESIS 0x89 */
/*  #define CH_LATIN_SMALL_LETTER_E_WITH_GRAVE 0x8a */
/*  #define CH_LATIN_SMALL_LETTER_I_WITH_DIAERESIS 0x8b */
/*  #define CH_LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX 0x8c */
/*  #define CH_LATIN_SMALL_LETTER_I_WITH_GRAVE 0x8d */
/*  #define CH_LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS 0x8e */
/*  #define CH_LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE 0x8f */
/*  #define CH_LATIN_CAPITAL_LETTER_E_WITH_ACUTE 0x90 */
/*  #define CH_LATIN_SMALL_LIGATURE_AE 0x91 */
/*  #define CH_LATIN_CAPITAL_LIGATURE_AE 0x92 */
/*  #define CH_LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX 0x93 */
/*  #define CH_LATIN_SMALL_LETTER_O_WITH_DIAERESIS 0x94 */
/*  #define CH_LATIN_SMALL_LETTER_O_WITH_GRAVE 0x95 */
/*  #define CH_LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX 0x96 */
/*  #define CH_LATIN_SMALL_LETTER_U_WITH_GRAVE 0x97 */
/*  #define CH_LATIN_SMALL_LETTER_Y_WITH_DIAERESIS 0x98 */
/*  #define CH_LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS 0x99 */
/*  #define CH_LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS 0x9a */
/*  #define CH_CENT_SIGN 0x9b */
/*  #define CH_POUND_SIGN 0x9c */
    #define CH_YEN_SIGN 0x9d
/*  #define CH_PESETA_SIGN 0x9e */
/*  #define CH_LATIN_SMALL_LETTER_F_WITH_HOOK 0x9f */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_ACUTE 0xa0 */
/*  #define CH_LATIN_SMALL_LETTER_I_WITH_ACUTE 0xa1 */
/*  #define CH_LATIN_SMALL_LETTER_O_WITH_ACUTE 0xa2 */
/*  #define CH_LATIN_SMALL_LETTER_U_WITH_ACUTE 0xa3 */
/*  #define CH_LATIN_SMALL_LETTER_N_WITH_TILDE 0xa4 */
/*  #define CH_LATIN_CAPITAL_LETTER_N_WITH_TILDE 0xa5 */
/*  #define CH_FEMININE_ORDINAL_INDICATOR 0xa6 */
/*  #define CH_MASCULINE_ORDINAL_INDICATOR 0xa7 */
/*  #define CH_INVERTED_QUESTION_MARK 0xa8 */
/*  #define CH_REVERSED_NOT_SIGN 0xa9 */
/*  #define CH_NOT_SIGN 0xaa */
/*  #define CH_VULGAR_FRACTION_ONE_HALF 0xab */
/*  #define CH_VULGAR_FRACTION_ONE_QUARTER 0xac */
/*  #define CH_INVERTED_EXCLAMATION_MARK 0xad */
    #define CH_LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK 0xae
    #define CH_RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK 0xaf
    #define CH_LIGHT_SHADE 0xb0
    #define CH_MEDIUM_SHADE 0xb1
    #define CH_DARK_SHADE 0xb2
    #define CH_BOX_DRAWINGS_LIGHT_VERTICAL 0xb3
    #define CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_LEFT 0xb4
    #define CH_BOX_DRAWINGS_VERTICAL_SINGLE_AND_LEFT_DOUBLE 0xb5
    #define CH_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_LEFT_SINGLE 0xb6
    #define CH_BOX_DRAWINGS_DOWN_DOUBLE_AND_LEFT_SINGLE 0xb7
    #define CH_BOX_DRAWINGS_DOWN_SINGLE_AND_LEFT_DOUBLE 0xb8
/*  #define CH_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_LEFT 0xb9 */
    #define CH_BOX_DRAWINGS_DOUBLE_VERTICAL 0xba
/*  #define CH_BOX_DRAWINGS_DOUBLE_DOWN_AND_LEFT 0xbb */
/*  #define CH_BOX_DRAWINGS_DOUBLE_UP_AND_LEFT 0xbc */
/*  #define CH_BOX_DRAWINGS_UP_DOUBLE_AND_LEFT_SINGLE 0xbd */
    #define CH_BOX_DRAWINGS_UP_SINGLE_AND_LEFT_DOUBLE 0xbe
    #define CH_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT 0xbf
    #define CH_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT 0xc0
/*  #define CH_BOX_DRAWINGS_LIGHT_UP_AND_HORIZONTAL 0xc1 */
/*  #define CH_BOX_DRAWINGS_LIGHT_DOWN_AND_HORIZONTAL 0xc2 */
    #define CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_RIGHT 0xc3
    #define CH_BOX_DRAWINGS_LIGHT_HORIZONTAL 0xc4
    #define CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_HORIZONTAL 0xc5
    #define CH_BOX_DRAWINGS_VERTICAL_SINGLE_AND_RIGHT_DOUBLE 0xc6
    #define CH_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_RIGHT_SINGLE 0xc7
/*  #define CH_BOX_DRAWINGS_DOUBLE_UP_AND_RIGHT 0xc8 */
/*  #define CH_BOX_DRAWINGS_DOUBLE_DOWN_AND_RIGHT 0xc9 */
/*  #define CH_BOX_DRAWINGS_DOUBLE_UP_AND_HORIZONTAL 0xca */
    #define CH_BOX_DRAWINGS_DOUBLE_DOWN_AND_HORIZONTAL 0xcb
/*  #define CH_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_RIGHT 0xcc */
    #define CH_BOX_DRAWINGS_DOUBLE_HORIZONTAL 0xcd
/*  #define CH_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_HORIZONTAL 0xce */
/*  #define CH_BOX_DRAWINGS_UP_SINGLE_AND_HORIZONTAL_DOUBLE 0xcf */
/*  #define CH_BOX_DRAWINGS_UP_DOUBLE_AND_HORIZONTAL_SINGLE 0xd0 */
    #define CH_BOX_DRAWINGS_DOWN_SINGLE_AND_HORIZONTAL_DOUBLE 0xd1
    #define CH_BOX_DRAWINGS_DOWN_DOUBLE_AND_HORIZONTAL_SINGLE 0xd2
/*  #define CH_BOX_DRAWINGS_UP_DOUBLE_AND_RIGHT_SINGLE 0xd3 */
/*  #define CH_BOX_DRAWINGS_UP_SINGLE_AND_RIGHT_DOUBLE 0xd4 */
/*  #define CH_BOX_DRAWINGS_DOWN_SINGLE_AND_RIGHT_DOUBLE 0xd5 */
/*  #define CH_BOX_DRAWINGS_DOWN_DOUBLE_AND_RIGHT_SINGLE 0xd6 */
/*  #define CH_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_HORIZONTAL_SINGLE 0xd7 */
/*  #define CH_BOX_DRAWINGS_VERTICAL_SINGLE_AND_HORIZONTAL_DOUBLE 0xd8 */
    #define CH_BOX_DRAWINGS_LIGHT_UP_AND_LEFT 0xd9
    #define CH_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT 0xda
    #define CH_FULL_BLOCK 0xdb
    #define CH_LOWER_HALF_BLOCK 0xdc
    #define CH_LEFT_HALF_BLOCK 0xdd
    #define CH_RIGHT_HALF_BLOCK 0xde
    #define CH_UPPER_HALF_BLOCK 0xdf
/*  #define CH_GREEK_SMALL_LETTER_ALPHA 0xe0 */
    #define CH_LATIN_SMALL_LETTER_SHARP_S 0xe1
/*  #define CH_GREEK_CAPITAL_LETTER_GAMMA 0xe2 */
/*  #define CH_GREEK_SMALL_LETTER_PI 0xe3 */
    #define CH_GREEK_CAPITAL_LETTER_SIGMA 0xe4
/*  #define CH_GREEK_SMALL_LETTER_SIGMA 0xe5 */
/*  #define CH_MICRO_SIGN 0xe6 */
/*  #define CH_GREEK_SMALL_LETTER_TAU 0xe7 */
    #define CH_GREEK_CAPITAL_LETTER_PHI 0xe8
/*  #define CH_GREEK_CAPITAL_LETTER_THETA 0xe9 */
/*  #define CH_GREEK_CAPITAL_LETTER_OMEGA 0xea */
/*  #define CH_GREEK_SMALL_LETTER_DELTA 0xeb */
    #define CH_INFINITY 0xec
/*  #define CH_GREEK_SMALL_LETTER_PHI 0xed */
/*  #define CH_GREEK_SMALL_LETTER_EPSILON 0xee */
/*  #define CH_INTERSECTION 0xef */
    #define CH_IDENTICAL_TO 0xf0
/*  #define CH_PLUS_MINUS_SIGN 0xf1 */
/*  #define CH_GREATER_THAN_OR_EQUAL_TO 0xf2 */
/*  #define CH_LESS_THAN_OR_EQUAL_TO 0xf3 */
/*  #define CH_TOP_HALF_INTEGRAL 0xf4 */
/*  #define CH_BOTTOM_HALF_INTEGRAL 0xf5 */
/*  #define CH_DIVISION_SIGN 0xf6 */
    #define CH_ALMOST_EQUAL_TO 0xf7
    #define CH_DEGREE_SIGN 0xf8
    #define CH_BULLET_OPERATOR 0xf9
/*  #define CH_MIDDLE_DOT 0xfa */
/*  #define CH_SQUARE_ROOT 0xfb */
/*  #define CH_SUPERSCRIPT_LATIN_SMALL_LETTER_N 0xfc */
/*  #define CH_SUPERSCRIPT_TWO 0xfd */
/*  #define CH_BLACK_SQUARE 0xfe */
/*  #define CH_NO_BREAK_SPACE 0xff */
#endif

#ifdef CH_USE_ASCII_HACK


  #define CH_BULLET			                            ACS_BULLET
  #define CH_WHITE_SUN_WITH_RAYS			            '*'
  #define CH_BLACK_RIGHT_POINTING_POINTER			    ACS_RARROW
  #define CH_BLACK_DOWN_POINTING_TRIANGLE			    ACS_DARROW

    #define CH_NULL ' '|A_REVERSE
/*  #define CH_START_OF_HEADING ? */
/*  #define CH_START_OF_TEXT ? */
/*  #define CH_END_OF_TEXT ? */
/*  #define CH_END_OF_TRANSMISSION ? */
/*  #define CH_ENQUIRY ? */
/*  #define CH_ACKNOWLEDGE ? */
    #define CH_BELL ACS_BULLET
/*  #define CH_BACKSPACE ? */
/*  #define CH_HORIZONTAL_TABULATION ? */
/*  #define CH_LINE_FEED ? */
/*  #define CH_VERTICAL_TABULATION ? */
/*  #define CH_FORM_FEED ? */
/*  #define CH_CARRIAGE_RETURN ? */
/*  #define CH_SHIFT_OUT ? */
    #define CH_SHIFT_IN '*'
    #define CH_DATA_LINK_ESCAPE ACS_RARROW
/*  #define CH_DEVICE_CONTROL_ONE ? */
/*  #define CH_DEVICE_CONTROL_TWO ? */
/*  #define CH_DEVICE_CONTROL_THREE ? */
/*  #define CH_DEVICE_CONTROL_FOUR ? */
/*  #define CH_NEGATIVE_ACKNOWLEDGE ? */
/*  #define CH_SYNCHRONOUS_IDLE ? */
/*  #define CH_END_OF_TRANSMISSION_BLOCK ? */
/*  #define CH_CANCEL ? */
/*  #define CH_END_OF_MEDIUM ? */
/*  #define CH_SUBSTITUTE ? */
/*  #define CH_ESCAPE ? */
/*  #define CH_FILE_SEPARATOR ? */
/*  #define CH_GROUP_SEPARATOR ? */
/*  #define CH_RECORD_SEPARATOR ? */
    #define CH_UNIT_SEPARATOR ACS_DARROW
/*  #define CH_SPACE ? */
/*  #define CH_EXCLAMATION_MARK ? */
/*  #define CH_QUOTATION_MARK ? */
/*  #define CH_NUMBER_SIGN ? */
/*  #define CH_DOLLAR_SIGN ? */
/*  #define CH_PERCENT_SIGN ? */
/*  #define CH_AMPERSAND ? */
/*  #define CH_APOSTROPHE ? */
/*  #define CH_LEFT_PARENTHESIS ? */
/*  #define CH_RIGHT_PARENTHESIS ? */
/*  #define CH_ASTERISK ? */
/*  #define CH_PLUS_SIGN ? */
/*  #define CH_COMMA ? */
/*  #define CH_HYPHEN_MINUS ? */
/*  #define CH_FULL_STOP ? */
/*  #define CH_SOLIDUS ? */
/*  #define CH_DIGIT_ZERO ? */
/*  #define CH_DIGIT_ONE ? */
/*  #define CH_DIGIT_TWO ? */
/*  #define CH_DIGIT_THREE ? */
/*  #define CH_DIGIT_FOUR ? */
/*  #define CH_DIGIT_FIVE ? */
/*  #define CH_DIGIT_SIX ? */
/*  #define CH_DIGIT_SEVEN ? */
/*  #define CH_DIGIT_EIGHT ? */
/*  #define CH_DIGIT_NINE ? */
/*  #define CH_COLON ? */
/*  #define CH_SEMICOLON ? */
/*  #define CH_LESS_THAN_SIGN ? */
/*  #define CH_EQUALS_SIGN ? */
/*  #define CH_GREATER_THAN_SIGN ? */
/*  #define CH_QUESTION_MARK ? */
/*  #define CH_COMMERCIAL_AT ? */
/*  #define CH_LATIN_CAPITAL_LETTER_A ? */
/*  #define CH_LATIN_CAPITAL_LETTER_B ? */
/*  #define CH_LATIN_CAPITAL_LETTER_C ? */
/*  #define CH_LATIN_CAPITAL_LETTER_D ? */
/*  #define CH_LATIN_CAPITAL_LETTER_E ? */
/*  #define CH_LATIN_CAPITAL_LETTER_F ? */
/*  #define CH_LATIN_CAPITAL_LETTER_G ? */
/*  #define CH_LATIN_CAPITAL_LETTER_H ? */
/*  #define CH_LATIN_CAPITAL_LETTER_I ? */
/*  #define CH_LATIN_CAPITAL_LETTER_J ? */
/*  #define CH_LATIN_CAPITAL_LETTER_K ? */
/*  #define CH_LATIN_CAPITAL_LETTER_L ? */
/*  #define CH_LATIN_CAPITAL_LETTER_M ? */
/*  #define CH_LATIN_CAPITAL_LETTER_N ? */
/*  #define CH_LATIN_CAPITAL_LETTER_O ? */
/*  #define CH_LATIN_CAPITAL_LETTER_P ? */
/*  #define CH_LATIN_CAPITAL_LETTER_Q ? */
/*  #define CH_LATIN_CAPITAL_LETTER_R ? */
/*  #define CH_LATIN_CAPITAL_LETTER_S ? */
/*  #define CH_LATIN_CAPITAL_LETTER_T ? */
/*  #define CH_LATIN_CAPITAL_LETTER_U ? */
/*  #define CH_LATIN_CAPITAL_LETTER_V ? */
/*  #define CH_LATIN_CAPITAL_LETTER_W ? */
/*  #define CH_LATIN_CAPITAL_LETTER_X ? */
/*  #define CH_LATIN_CAPITAL_LETTER_Y ? */
/*  #define CH_LATIN_CAPITAL_LETTER_Z ? */
/*  #define CH_LEFT_SQUARE_BRACKET ? */
/*  #define CH_REVERSE_SOLIDUS ? */
/*  #define CH_RIGHT_SQUARE_BRACKET ? */
/*  #define CH_CIRCUMFLEX_ACCENT ? */
/*  #define CH_LOW_LINE ? */
/*  #define CH_GRAVE_ACCENT ? */
/*  #define CH_LATIN_SMALL_LETTER_A ? */
/*  #define CH_LATIN_SMALL_LETTER_B ? */
/*  #define CH_LATIN_SMALL_LETTER_C ? */
/*  #define CH_LATIN_SMALL_LETTER_D ? */
/*  #define CH_LATIN_SMALL_LETTER_E ? */
/*  #define CH_LATIN_SMALL_LETTER_F ? */
/*  #define CH_LATIN_SMALL_LETTER_G ? */
/*  #define CH_LATIN_SMALL_LETTER_H ? */
/*  #define CH_LATIN_SMALL_LETTER_I ? */
/*  #define CH_LATIN_SMALL_LETTER_J ? */
/*  #define CH_LATIN_SMALL_LETTER_K ? */
/*  #define CH_LATIN_SMALL_LETTER_L ? */
/*  #define CH_LATIN_SMALL_LETTER_M ? */
/*  #define CH_LATIN_SMALL_LETTER_N ? */
/*  #define CH_LATIN_SMALL_LETTER_O ? */
/*  #define CH_LATIN_SMALL_LETTER_P ? */
/*  #define CH_LATIN_SMALL_LETTER_Q ? */
/*  #define CH_LATIN_SMALL_LETTER_R ? */
/*  #define CH_LATIN_SMALL_LETTER_S ? */
/*  #define CH_LATIN_SMALL_LETTER_T ? */
/*  #define CH_LATIN_SMALL_LETTER_U ? */
/*  #define CH_LATIN_SMALL_LETTER_V ? */
/*  #define CH_LATIN_SMALL_LETTER_W ? */
/*  #define CH_LATIN_SMALL_LETTER_X ? */
/*  #define CH_LATIN_SMALL_LETTER_Y ? */
/*  #define CH_LATIN_SMALL_LETTER_Z ? */
/*  #define CH_LEFT_CURLY_BRACKET ? */
/*  #define CH_VERTICAL_LINE ? */
/*  #define CH_RIGHT_CURLY_BRACKET ? */
/*  #define CH_TILDE ? */
/*  #define CH_DELETE ? */
/*  #define CH_LATIN_CAPITAL_LETTER_C_WITH_CEDILLA ? */
/*  #define CH_LATIN_SMALL_LETTER_U_WITH_DIAERESIS ? */
/*  #define CH_LATIN_SMALL_LETTER_E_WITH_ACUTE ? */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX ? */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_DIAERESIS ? */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_GRAVE ? */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_RING_ABOVE ? */
/*  #define CH_LATIN_SMALL_LETTER_C_WITH_CEDILLA ? */
/*  #define CH_LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX ? */
/*  #define CH_LATIN_SMALL_LETTER_E_WITH_DIAERESIS ? */
/*  #define CH_LATIN_SMALL_LETTER_E_WITH_GRAVE ? */
/*  #define CH_LATIN_SMALL_LETTER_I_WITH_DIAERESIS ? */
/*  #define CH_LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX ? */
/*  #define CH_LATIN_SMALL_LETTER_I_WITH_GRAVE ? */
/*  #define CH_LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS ? */
/*  #define CH_LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE ? */
/*  #define CH_LATIN_CAPITAL_LETTER_E_WITH_ACUTE ? */
/*  #define CH_LATIN_SMALL_LIGATURE_AE ? */
/*  #define CH_LATIN_CAPITAL_LIGATURE_AE ? */
/*  #define CH_LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX ? */
/*  #define CH_LATIN_SMALL_LETTER_O_WITH_DIAERESIS ? */
/*  #define CH_LATIN_SMALL_LETTER_O_WITH_GRAVE ? */
/*  #define CH_LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX ? */
/*  #define CH_LATIN_SMALL_LETTER_U_WITH_GRAVE ? */
/*  #define CH_LATIN_SMALL_LETTER_Y_WITH_DIAERESIS ? */
/*  #define CH_LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS ? */
/*  #define CH_LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS ? */
/*  #define CH_CENT_SIGN ? */
/*  #define CH_POUND_SIGN ? */
    #define CH_YEN_SIGN 165
/*  #define CH_PESETA_SIGN ? */
/*  #define CH_LATIN_SMALL_LETTER_F_WITH_HOOK ? */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_ACUTE ? */
/*  #define CH_LATIN_SMALL_LETTER_I_WITH_ACUTE ? */
/*  #define CH_LATIN_SMALL_LETTER_O_WITH_ACUTE ? */
/*  #define CH_LATIN_SMALL_LETTER_U_WITH_ACUTE ? */
/*  #define CH_LATIN_SMALL_LETTER_N_WITH_TILDE ? */
/*  #define CH_LATIN_CAPITAL_LETTER_N_WITH_TILDE ? */
/*  #define CH_FEMININE_ORDINAL_INDICATOR ? */
/*  #define CH_MASCULINE_ORDINAL_INDICATOR ? */
/*  #define CH_INVERTED_QUESTION_MARK ? */
/*  #define CH_REVERSED_NOT_SIGN ? */
/*  #define CH_NOT_SIGN ? */
/*  #define CH_VULGAR_FRACTION_ONE_HALF ? */
/*  #define CH_VULGAR_FRACTION_ONE_QUARTER ? */
/*  #define CH_INVERTED_EXCLAMATION_MARK ? */
    #define CH_LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK '<'
    #define CH_RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK '>'
    #define CH_LIGHT_SHADE ':'
    #define CH_MEDIUM_SHADE ACS_BOARD
    #define CH_DARK_SHADE ACS_CKBOARD
    #define CH_BOX_DRAWINGS_LIGHT_VERTICAL ACS_VLINE
    #define CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_LEFT ACS_RTEE
    #define CH_BOX_DRAWINGS_VERTICAL_SINGLE_AND_LEFT_DOUBLE ACS_RTEE
    #define CH_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_LEFT_SINGLE ACS_RTEE
    #define CH_BOX_DRAWINGS_DOWN_DOUBLE_AND_LEFT_SINGLE ACS_URCORNER
    #define CH_BOX_DRAWINGS_DOWN_SINGLE_AND_LEFT_DOUBLE ACS_URCORNER
/*  #define CH_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_LEFT ? */
    #define CH_BOX_DRAWINGS_DOUBLE_VERTICAL ACS_VLINE
/*  #define CH_BOX_DRAWINGS_DOUBLE_DOWN_AND_LEFT ? */
/*  #define CH_BOX_DRAWINGS_DOUBLE_UP_AND_LEFT ? */
/*  #define CH_BOX_DRAWINGS_UP_DOUBLE_AND_LEFT_SINGLE ? */
    #define CH_BOX_DRAWINGS_UP_SINGLE_AND_LEFT_DOUBLE ACS_URCORNER
    #define CH_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT ACS_LLCORNER
    #define CH_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT ACS_LLCORNER
/*  #define CH_BOX_DRAWINGS_LIGHT_UP_AND_HORIZONTAL ? */
/*  #define CH_BOX_DRAWINGS_LIGHT_DOWN_AND_HORIZONTAL ? */
    #define CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_RIGHT ACS_LTEE
    #define CH_BOX_DRAWINGS_LIGHT_HORIZONTAL ACS_HLINE
    #define CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_HORIZONTAL ACS_PLUS
    #define CH_BOX_DRAWINGS_VERTICAL_SINGLE_AND_RIGHT_DOUBLE ACS_LTEE
    #define CH_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_RIGHT_SINGLE ACS_LTEE
/*  #define CH_BOX_DRAWINGS_DOUBLE_UP_AND_RIGHT ? */
/*  #define CH_BOX_DRAWINGS_DOUBLE_DOWN_AND_RIGHT ? */
/*  #define CH_BOX_DRAWINGS_DOUBLE_UP_AND_HORIZONTAL ? */
    #define CH_BOX_DRAWINGS_DOUBLE_DOWN_AND_HORIZONTAL ACS_TTEE
/*  #define CH_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_RIGHT ? */
    #define CH_BOX_DRAWINGS_DOUBLE_HORIZONTAL ACS_HLINE
/*  #define CH_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_HORIZONTAL ? */
/*  #define CH_BOX_DRAWINGS_UP_SINGLE_AND_HORIZONTAL_DOUBLE ? */
/*  #define CH_BOX_DRAWINGS_UP_DOUBLE_AND_HORIZONTAL_SINGLE ? */
    #define CH_BOX_DRAWINGS_DOWN_SINGLE_AND_HORIZONTAL_DOUBLE ACS_TTEE
    #define CH_BOX_DRAWINGS_DOWN_DOUBLE_AND_HORIZONTAL_SINGLE ACS_TTEE
/*  #define CH_BOX_DRAWINGS_UP_DOUBLE_AND_RIGHT_SINGLE ? */
/*  #define CH_BOX_DRAWINGS_UP_SINGLE_AND_RIGHT_DOUBLE ? */
/*  #define CH_BOX_DRAWINGS_DOWN_SINGLE_AND_RIGHT_DOUBLE ? */
/*  #define CH_BOX_DRAWINGS_DOWN_DOUBLE_AND_RIGHT_SINGLE ? */
/*  #define CH_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_HORIZONTAL_SINGLE ? */
/*  #define CH_BOX_DRAWINGS_VERTICAL_SINGLE_AND_HORIZONTAL_DOUBLE ? */
    #define CH_BOX_DRAWINGS_LIGHT_UP_AND_LEFT ACS_LRCORNER
    #define CH_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT ACS_ULCORNER
    #define CH_FULL_BLOCK (' '|A_REVERSE)
    #define CH_LOWER_HALF_BLOCK '_'
    #define CH_LEFT_HALF_BLOCK '|'
    #define CH_RIGHT_HALF_BLOCK '|'
    #define CH_UPPER_HALF_BLOCK '~'
/*  #define CH_GREEK_SMALL_LETTER_ALPHA ? */
    #define CH_LATIN_SMALL_LETTER_SHARP_S 'B'
/*  #define CH_GREEK_CAPITAL_LETTER_GAMMA ? */
/*  #define CH_GREEK_SMALL_LETTER_PI ? */
    #define CH_GREEK_CAPITAL_LETTER_SIGMA 'E'
/*  #define CH_GREEK_SMALL_LETTER_SIGMA ? */
/*  #define CH_MICRO_SIGN ? */
/*  #define CH_GREEK_SMALL_LETTER_TAU ? */
    #define CH_GREEK_CAPITAL_LETTER_PHI 'Q'
/*  #define CH_GREEK_CAPITAL_LETTER_THETA ? */
/*  #define CH_GREEK_CAPITAL_LETTER_OMEGA ? */
/*  #define CH_GREEK_SMALL_LETTER_DELTA ? */
    #define CH_INFINITY '8'
/*  #define CH_GREEK_SMALL_LETTER_PHI ? */
/*  #define CH_GREEK_SMALL_LETTER_EPSILON ? */
/*  #define CH_INTERSECTION ? */
    #define CH_IDENTICAL_TO '='
/*  #define CH_PLUS_MINUS_SIGN ? */
/*  #define CH_GREATER_THAN_OR_EQUAL_TO ? */
/*  #define CH_LESS_THAN_OR_EQUAL_TO ? */
/*  #define CH_TOP_HALF_INTEGRAL ? */
/*  #define CH_BOTTOM_HALF_INTEGRAL ? */
/*  #define CH_DIVISION_SIGN ? */
    #define CH_ALMOST_EQUAL_TO '='
    #define CH_DEGREE_SIGN ACS_DEGREE
    #define CH_BULLET_OPERATOR ACS_BULLET
/*  #define CH_MIDDLE_DOT ? */
/*  #define CH_SQUARE_ROOT ? */
/*  #define CH_SUPERSCRIPT_LATIN_SMALL_LETTER_N ? */
/*  #define CH_SUPERSCRIPT_TWO ? */
/*  #define CH_BLACK_SQUARE ? */
/*  #define CH_NO_BREAK_SPACE ? */
#endif

#ifdef CH_USE_UNICODE
    #define CH_NULL 0x0000
/*  #define CH_START_OF_HEADING 0x0001 */
/*  #define CH_START_OF_TEXT 0x0002 */
/*  #define CH_END_OF_TEXT 0x0003 */
/*  #define CH_END_OF_TRANSMISSION 0x0004 */
/*  #define CH_ENQUIRY 0x0005 */
/*  #define CH_ACKNOWLEDGE 0x0006 */
    #define CH_BELL 0x0007
/*  #define CH_BACKSPACE 0x0008 */
/*  #define CH_HORIZONTAL_TABULATION 0x0009 */
/*  #define CH_LINE_FEED 0x000a */
/*  #define CH_VERTICAL_TABULATION 0x000b */
/*  #define CH_FORM_FEED 0x000c */
/*  #define CH_CARRIAGE_RETURN 0x000d */
/*  #define CH_SHIFT_OUT 0x000e */
    #define CH_SHIFT_IN 0x000f
    #define CH_DATA_LINK_ESCAPE 0x0010
/*  #define CH_DEVICE_CONTROL_ONE 0x0011 */
/*  #define CH_DEVICE_CONTROL_TWO 0x0012 */
/*  #define CH_DEVICE_CONTROL_THREE 0x0013 */
/*  #define CH_DEVICE_CONTROL_FOUR 0x0014 */
/*  #define CH_NEGATIVE_ACKNOWLEDGE 0x0015 */
/*  #define CH_SYNCHRONOUS_IDLE 0x0016 */
/*  #define CH_END_OF_TRANSMISSION_BLOCK 0x0017 */
/*  #define CH_CANCEL 0x0018 */
/*  #define CH_END_OF_MEDIUM 0x0019 */
/*  #define CH_SUBSTITUTE 0x001a */
/*  #define CH_ESCAPE 0x001b */
/*  #define CH_FILE_SEPARATOR 0x001c */
/*  #define CH_GROUP_SEPARATOR 0x001d */
/*  #define CH_RECORD_SEPARATOR 0x001e */
    #define CH_UNIT_SEPARATOR 0x001f
/*  #define CH_SPACE 0x0020 */
/*  #define CH_EXCLAMATION_MARK 0x0021 */
/*  #define CH_QUOTATION_MARK 0x0022 */
/*  #define CH_NUMBER_SIGN 0x0023 */
/*  #define CH_DOLLAR_SIGN 0x0024 */
/*  #define CH_PERCENT_SIGN 0x0025 */
/*  #define CH_AMPERSAND 0x0026 */
/*  #define CH_APOSTROPHE 0x0027 */
/*  #define CH_LEFT_PARENTHESIS 0x0028 */
/*  #define CH_RIGHT_PARENTHESIS 0x0029 */
/*  #define CH_ASTERISK 0x002a */
/*  #define CH_PLUS_SIGN 0x002b */
/*  #define CH_COMMA 0x002c */
/*  #define CH_HYPHEN_MINUS 0x002d */
/*  #define CH_FULL_STOP 0x002e */
/*  #define CH_SOLIDUS 0x002f */
/*  #define CH_DIGIT_ZERO 0x0030 */
/*  #define CH_DIGIT_ONE 0x0031 */
/*  #define CH_DIGIT_TWO 0x0032 */
/*  #define CH_DIGIT_THREE 0x0033 */
/*  #define CH_DIGIT_FOUR 0x0034 */
/*  #define CH_DIGIT_FIVE 0x0035 */
/*  #define CH_DIGIT_SIX 0x0036 */
/*  #define CH_DIGIT_SEVEN 0x0037 */
/*  #define CH_DIGIT_EIGHT 0x0038 */
/*  #define CH_DIGIT_NINE 0x0039 */
/*  #define CH_COLON 0x003a */
/*  #define CH_SEMICOLON 0x003b */
/*  #define CH_LESS_THAN_SIGN 0x003c */
/*  #define CH_EQUALS_SIGN 0x003d */
/*  #define CH_GREATER_THAN_SIGN 0x003e */
/*  #define CH_QUESTION_MARK 0x003f */
/*  #define CH_COMMERCIAL_AT 0x0040 */
/*  #define CH_LATIN_CAPITAL_LETTER_A 0x0041 */
/*  #define CH_LATIN_CAPITAL_LETTER_B 0x0042 */
/*  #define CH_LATIN_CAPITAL_LETTER_C 0x0043 */
/*  #define CH_LATIN_CAPITAL_LETTER_D 0x0044 */
/*  #define CH_LATIN_CAPITAL_LETTER_E 0x0045 */
/*  #define CH_LATIN_CAPITAL_LETTER_F 0x0046 */
/*  #define CH_LATIN_CAPITAL_LETTER_G 0x0047 */
/*  #define CH_LATIN_CAPITAL_LETTER_H 0x0048 */
/*  #define CH_LATIN_CAPITAL_LETTER_I 0x0049 */
/*  #define CH_LATIN_CAPITAL_LETTER_J 0x004a */
/*  #define CH_LATIN_CAPITAL_LETTER_K 0x004b */
/*  #define CH_LATIN_CAPITAL_LETTER_L 0x004c */
/*  #define CH_LATIN_CAPITAL_LETTER_M 0x004d */
/*  #define CH_LATIN_CAPITAL_LETTER_N 0x004e */
/*  #define CH_LATIN_CAPITAL_LETTER_O 0x004f */
/*  #define CH_LATIN_CAPITAL_LETTER_P 0x0050 */
/*  #define CH_LATIN_CAPITAL_LETTER_Q 0x0051 */
/*  #define CH_LATIN_CAPITAL_LETTER_R 0x0052 */
/*  #define CH_LATIN_CAPITAL_LETTER_S 0x0053 */
/*  #define CH_LATIN_CAPITAL_LETTER_T 0x0054 */
/*  #define CH_LATIN_CAPITAL_LETTER_U 0x0055 */
/*  #define CH_LATIN_CAPITAL_LETTER_V 0x0056 */
/*  #define CH_LATIN_CAPITAL_LETTER_W 0x0057 */
/*  #define CH_LATIN_CAPITAL_LETTER_X 0x0058 */
/*  #define CH_LATIN_CAPITAL_LETTER_Y 0x0059 */
/*  #define CH_LATIN_CAPITAL_LETTER_Z 0x005a */
/*  #define CH_LEFT_SQUARE_BRACKET 0x005b */
/*  #define CH_REVERSE_SOLIDUS 0x005c */
/*  #define CH_RIGHT_SQUARE_BRACKET 0x005d */
/*  #define CH_CIRCUMFLEX_ACCENT 0x005e */
/*  #define CH_LOW_LINE 0x005f */
/*  #define CH_GRAVE_ACCENT 0x0060 */
/*  #define CH_LATIN_SMALL_LETTER_A 0x0061 */
/*  #define CH_LATIN_SMALL_LETTER_B 0x0062 */
/*  #define CH_LATIN_SMALL_LETTER_C 0x0063 */
/*  #define CH_LATIN_SMALL_LETTER_D 0x0064 */
/*  #define CH_LATIN_SMALL_LETTER_E 0x0065 */
/*  #define CH_LATIN_SMALL_LETTER_F 0x0066 */
/*  #define CH_LATIN_SMALL_LETTER_G 0x0067 */
/*  #define CH_LATIN_SMALL_LETTER_H 0x0068 */
/*  #define CH_LATIN_SMALL_LETTER_I 0x0069 */
/*  #define CH_LATIN_SMALL_LETTER_J 0x006a */
/*  #define CH_LATIN_SMALL_LETTER_K 0x006b */
/*  #define CH_LATIN_SMALL_LETTER_L 0x006c */
/*  #define CH_LATIN_SMALL_LETTER_M 0x006d */
/*  #define CH_LATIN_SMALL_LETTER_N 0x006e */
/*  #define CH_LATIN_SMALL_LETTER_O 0x006f */
/*  #define CH_LATIN_SMALL_LETTER_P 0x0070 */
/*  #define CH_LATIN_SMALL_LETTER_Q 0x0071 */
/*  #define CH_LATIN_SMALL_LETTER_R 0x0072 */
/*  #define CH_LATIN_SMALL_LETTER_S 0x0073 */
/*  #define CH_LATIN_SMALL_LETTER_T 0x0074 */
/*  #define CH_LATIN_SMALL_LETTER_U 0x0075 */
/*  #define CH_LATIN_SMALL_LETTER_V 0x0076 */
/*  #define CH_LATIN_SMALL_LETTER_W 0x0077 */
/*  #define CH_LATIN_SMALL_LETTER_X 0x0078 */
/*  #define CH_LATIN_SMALL_LETTER_Y 0x0079 */
/*  #define CH_LATIN_SMALL_LETTER_Z 0x007a */
/*  #define CH_LEFT_CURLY_BRACKET 0x007b */
/*  #define CH_VERTICAL_LINE 0x007c */
/*  #define CH_RIGHT_CURLY_BRACKET 0x007d */
/*  #define CH_TILDE 0x007e */
/*  #define CH_DELETE 0x007f */
/*  #define CH_LATIN_CAPITAL_LETTER_C_WITH_CEDILLA 0x00c7 */
/*  #define CH_LATIN_SMALL_LETTER_U_WITH_DIAERESIS 0x00fc */
/*  #define CH_LATIN_SMALL_LETTER_E_WITH_ACUTE 0x00e9 */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX 0x00e2 */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_DIAERESIS 0x00e4 */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_GRAVE 0x00e0 */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_RING_ABOVE 0x00e5 */
/*  #define CH_LATIN_SMALL_LETTER_C_WITH_CEDILLA 0x00e7 */
/*  #define CH_LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX 0x00ea */
/*  #define CH_LATIN_SMALL_LETTER_E_WITH_DIAERESIS 0x00eb */
/*  #define CH_LATIN_SMALL_LETTER_E_WITH_GRAVE 0x00e8 */
/*  #define CH_LATIN_SMALL_LETTER_I_WITH_DIAERESIS 0x00ef */
/*  #define CH_LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX 0x00ee */
/*  #define CH_LATIN_SMALL_LETTER_I_WITH_GRAVE 0x00ec */
/*  #define CH_LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS 0x00c4 */
/*  #define CH_LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE 0x00c5 */
/*  #define CH_LATIN_CAPITAL_LETTER_E_WITH_ACUTE 0x00c9 */
/*  #define CH_LATIN_SMALL_LIGATURE_AE 0x00e6 */
/*  #define CH_LATIN_CAPITAL_LIGATURE_AE 0x00c6 */
/*  #define CH_LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX 0x00f4 */
/*  #define CH_LATIN_SMALL_LETTER_O_WITH_DIAERESIS 0x00f6 */
/*  #define CH_LATIN_SMALL_LETTER_O_WITH_GRAVE 0x00f2 */
/*  #define CH_LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX 0x00fb */
/*  #define CH_LATIN_SMALL_LETTER_U_WITH_GRAVE 0x00f9 */
/*  #define CH_LATIN_SMALL_LETTER_Y_WITH_DIAERESIS 0x00ff */
/*  #define CH_LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS 0x00d6 */
/*  #define CH_LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS 0x00dc */
/*  #define CH_CENT_SIGN 0x00a2 */
/*  #define CH_POUND_SIGN 0x00a3 */
    #define CH_YEN_SIGN 0x00a5
/*  #define CH_PESETA_SIGN 0x20a7 */
/*  #define CH_LATIN_SMALL_LETTER_F_WITH_HOOK 0x0192 */
/*  #define CH_LATIN_SMALL_LETTER_A_WITH_ACUTE 0x00e1 */
/*  #define CH_LATIN_SMALL_LETTER_I_WITH_ACUTE 0x00ed */
/*  #define CH_LATIN_SMALL_LETTER_O_WITH_ACUTE 0x00f3 */
/*  #define CH_LATIN_SMALL_LETTER_U_WITH_ACUTE 0x00fa */
/*  #define CH_LATIN_SMALL_LETTER_N_WITH_TILDE 0x00f1 */
/*  #define CH_LATIN_CAPITAL_LETTER_N_WITH_TILDE 0x00d1 */
/*  #define CH_FEMININE_ORDINAL_INDICATOR 0x00aa */
/*  #define CH_MASCULINE_ORDINAL_INDICATOR 0x00ba */
/*  #define CH_INVERTED_QUESTION_MARK 0x00bf */
/*  #define CH_REVERSED_NOT_SIGN 0x2310 */
/*  #define CH_NOT_SIGN 0x00ac */
/*  #define CH_VULGAR_FRACTION_ONE_HALF 0x00bd */
/*  #define CH_VULGAR_FRACTION_ONE_QUARTER 0x00bc */
/*  #define CH_INVERTED_EXCLAMATION_MARK 0x00a1 */
    #define CH_LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK 0x00ab
    #define CH_RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK 0x00bb
    #define CH_LIGHT_SHADE 0x2591
    #define CH_MEDIUM_SHADE 0x2592
    #define CH_DARK_SHADE 0x2593
    #define CH_BOX_DRAWINGS_LIGHT_VERTICAL 0x2502
    #define CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_LEFT 0x2524
    #define CH_BOX_DRAWINGS_VERTICAL_SINGLE_AND_LEFT_DOUBLE 0x2561
    #define CH_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_LEFT_SINGLE 0x2562
    #define CH_BOX_DRAWINGS_DOWN_DOUBLE_AND_LEFT_SINGLE 0x2556
    #define CH_BOX_DRAWINGS_DOWN_SINGLE_AND_LEFT_DOUBLE 0x2555
/*  #define CH_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_LEFT 0x2563 */
    #define CH_BOX_DRAWINGS_DOUBLE_VERTICAL 0x2551
/*  #define CH_BOX_DRAWINGS_DOUBLE_DOWN_AND_LEFT 0x2557 */
/*  #define CH_BOX_DRAWINGS_DOUBLE_UP_AND_LEFT 0x255d */
/*  #define CH_BOX_DRAWINGS_UP_DOUBLE_AND_LEFT_SINGLE 0x255c */
    #define CH_BOX_DRAWINGS_UP_SINGLE_AND_LEFT_DOUBLE 0x255b
    #define CH_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT 0x2510
    #define CH_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT 0x2514
/*  #define CH_BOX_DRAWINGS_LIGHT_UP_AND_HORIZONTAL 0x2534 */
/*  #define CH_BOX_DRAWINGS_LIGHT_DOWN_AND_HORIZONTAL 0x252c */
    #define CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_RIGHT 0x251c
    #define CH_BOX_DRAWINGS_LIGHT_HORIZONTAL 0x2500
    #define CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_HORIZONTAL 0x253c
    #define CH_BOX_DRAWINGS_VERTICAL_SINGLE_AND_RIGHT_DOUBLE 0x255e
    #define CH_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_RIGHT_SINGLE 0x255f
/*  #define CH_BOX_DRAWINGS_DOUBLE_UP_AND_RIGHT 0x255a */
/*  #define CH_BOX_DRAWINGS_DOUBLE_DOWN_AND_RIGHT 0x2554 */
/*  #define CH_BOX_DRAWINGS_DOUBLE_UP_AND_HORIZONTAL 0x2569 */
    #define CH_BOX_DRAWINGS_DOUBLE_DOWN_AND_HORIZONTAL 0x2566
/*  #define CH_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_RIGHT 0x2560 */
    #define CH_BOX_DRAWINGS_DOUBLE_HORIZONTAL 0x2550
/*  #define CH_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_HORIZONTAL 0x256c */
/*  #define CH_BOX_DRAWINGS_UP_SINGLE_AND_HORIZONTAL_DOUBLE 0x2567 */
/*  #define CH_BOX_DRAWINGS_UP_DOUBLE_AND_HORIZONTAL_SINGLE 0x2568 */
    #define CH_BOX_DRAWINGS_DOWN_SINGLE_AND_HORIZONTAL_DOUBLE 0x2564
    #define CH_BOX_DRAWINGS_DOWN_DOUBLE_AND_HORIZONTAL_SINGLE 0x2565
/*  #define CH_BOX_DRAWINGS_UP_DOUBLE_AND_RIGHT_SINGLE 0x2559 */
/*  #define CH_BOX_DRAWINGS_UP_SINGLE_AND_RIGHT_DOUBLE 0x2558 */
/*  #define CH_BOX_DRAWINGS_DOWN_SINGLE_AND_RIGHT_DOUBLE 0x2552 */
/*  #define CH_BOX_DRAWINGS_DOWN_DOUBLE_AND_RIGHT_SINGLE 0x2553 */
/*  #define CH_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_HORIZONTAL_SINGLE 0x256b */
/*  #define CH_BOX_DRAWINGS_VERTICAL_SINGLE_AND_HORIZONTAL_DOUBLE 0x256a */
    #define CH_BOX_DRAWINGS_LIGHT_UP_AND_LEFT 0x2518
    #define CH_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT 0x250c
    #define CH_FULL_BLOCK 0x2588
    #define CH_LOWER_HALF_BLOCK 0x2584
    #define CH_LEFT_HALF_BLOCK 0x258c
    #define CH_RIGHT_HALF_BLOCK 0x2590
    #define CH_UPPER_HALF_BLOCK 0x2580
/*  #define CH_GREEK_SMALL_LETTER_ALPHA 0x03b1 */
    #define CH_LATIN_SMALL_LETTER_SHARP_S 0x00df
/*  #define CH_GREEK_CAPITAL_LETTER_GAMMA 0x0393 */
/*  #define CH_GREEK_SMALL_LETTER_PI 0x03c0 */
    #define CH_GREEK_CAPITAL_LETTER_SIGMA 0x03a3
/*  #define CH_GREEK_SMALL_LETTER_SIGMA 0x03c3 */
/*  #define CH_MICRO_SIGN 0x00b5 */
/*  #define CH_GREEK_SMALL_LETTER_TAU 0x03c4 */
    #define CH_GREEK_CAPITAL_LETTER_PHI 0x03a6
/*  #define CH_GREEK_CAPITAL_LETTER_THETA 0x0398 */
/*  #define CH_GREEK_CAPITAL_LETTER_OMEGA 0x03a9 */
/*  #define CH_GREEK_SMALL_LETTER_DELTA 0x03b4 */
    #define CH_INFINITY 0x221e
/*  #define CH_GREEK_SMALL_LETTER_PHI 0x03c6 */
/*  #define CH_GREEK_SMALL_LETTER_EPSILON 0x03b5 */
/*  #define CH_INTERSECTION 0x2229 */
    #define CH_IDENTICAL_TO 0x2261
/*  #define CH_PLUS_MINUS_SIGN 0x00b1 */
/*  #define CH_GREATER_THAN_OR_EQUAL_TO 0x2265 */
/*  #define CH_LESS_THAN_OR_EQUAL_TO 0x2264 */
/*  #define CH_TOP_HALF_INTEGRAL 0x2320 */
/*  #define CH_BOTTOM_HALF_INTEGRAL 0x2321 */
/*  #define CH_DIVISION_SIGN 0x00f7 */
    #define CH_ALMOST_EQUAL_TO 0x2248
    #define CH_DEGREE_SIGN 0x00b0
    #define CH_BULLET_OPERATOR 0x2219
/*  #define CH_MIDDLE_DOT 0x00b7 */
/*  #define CH_SQUARE_ROOT 0x221a */
/*  #define CH_SUPERSCRIPT_LATIN_SMALL_LETTER_N 0x207f */
/*  #define CH_SUPERSCRIPT_TWO 0x00b2 */
/*  #define CH_BLACK_SQUARE 0x25a0 */
/*  #define CH_NO_BREAK_SPACE 0x00a0 */

#endif


struct graphicschar
{
	unsigned int native_code;
	unsigned int cp437_code;
};

#ifdef CURSES_GRAPHICS

struct graphicschar gchar[] = {
{                                                         CH_NULL,      0,},
{   1, /*                                     CH_START_OF_HEADING, */   1,}, /* Not Used */
{   2, /*                                        CH_START_OF_TEXT, */   2,}, /* Not Used */
{   3, /*                                          CH_END_OF_TEXT, */   3,}, /* Not Used */
{   4, /*                                  CH_END_OF_TRANSMISSION, */   4,}, /* Not Used */
{   5, /*                                              CH_ENQUIRY, */   5,}, /* Not Used */
{   6, /*                                          CH_ACKNOWLEDGE, */   6,}, /* Not Used */
{                                                         CH_BELL,      7,},
{   8, /*                                            CH_BACKSPACE, */   8,}, /* Not Used */
{   9, /*                                CH_HORIZONTAL_TABULATION, */   9,}, /* Not Used */
{  10, /*                                            CH_LINE_FEED, */  10,}, /* Not Used */
{  11, /*                                  CH_VERTICAL_TABULATION, */  11,}, /* Not Used */
{  12, /*                                            CH_FORM_FEED, */  12,}, /* Not Used */
{  13, /*                                      CH_CARRIAGE_RETURN, */  13,}, /* Not Used */
{  14, /*                                            CH_SHIFT_OUT, */  14,}, /* Not Used */
{                                                     CH_SHIFT_IN,     15,},
{                                             CH_DATA_LINK_ESCAPE,     16,},
{  17, /*                                   CH_DEVICE_CONTROL_ONE, */  17,}, /* Not Used */
{  18, /*                                   CH_DEVICE_CONTROL_TWO, */  18,}, /* Not Used */
{  19, /*                                 CH_DEVICE_CONTROL_THREE, */  19,}, /* Not Used */
{  20, /*                                  CH_DEVICE_CONTROL_FOUR, */  20,}, /* Not Used */
{  21, /*                                 CH_NEGATIVE_ACKNOWLEDGE, */  21,}, /* Not Used */
{  22, /*                                     CH_SYNCHRONOUS_IDLE, */  22,}, /* Not Used */
{  23, /*                            CH_END_OF_TRANSMISSION_BLOCK, */  23,}, /* Not Used */
{  24, /*                                               CH_CANCEL, */  24,}, /* Not Used */
{  25, /*                                        CH_END_OF_MEDIUM, */  25,}, /* Not Used */
{  26, /*                                           CH_SUBSTITUTE, */  26,}, /* Not Used */
{  27, /*                                               CH_ESCAPE, */  27,}, /* Not Used */
{  28, /*                                       CH_FILE_SEPARATOR, */  28,}, /* Not Used */
{  29, /*                                      CH_GROUP_SEPARATOR, */  29,}, /* Not Used */
{  30, /*                                     CH_RECORD_SEPARATOR, */  30,}, /* Not Used */
{                                               CH_UNIT_SEPARATOR,     31,},
{  32, /*                                                CH_SPACE, */  32,}, /* Not Used */
{  33, /*                                     CH_EXCLAMATION_MARK, */  33,}, /* Not Used */
{  34, /*                                       CH_QUOTATION_MARK, */  34,}, /* Not Used */
{  35, /*                                          CH_NUMBER_SIGN, */  35,}, /* Not Used */
{  36, /*                                          CH_DOLLAR_SIGN, */  36,}, /* Not Used */
{  37, /*                                         CH_PERCENT_SIGN, */  37,}, /* Not Used */
{  38, /*                                            CH_AMPERSAND, */  38,}, /* Not Used */
{  39, /*                                           CH_APOSTROPHE, */  39,}, /* Not Used */
{  40, /*                                     CH_LEFT_PARENTHESIS, */  40,}, /* Not Used */
{  41, /*                                    CH_RIGHT_PARENTHESIS, */  41,}, /* Not Used */
{  42, /*                                             CH_ASTERISK, */  42,}, /* Not Used */
{  43, /*                                            CH_PLUS_SIGN, */  43,}, /* Not Used */
{  44, /*                                                CH_COMMA, */  44,}, /* Not Used */
{  45, /*                                         CH_HYPHEN_MINUS, */  45,}, /* Not Used */
{  46, /*                                            CH_FULL_STOP, */  46,}, /* Not Used */
{  47, /*                                              CH_SOLIDUS, */  47,}, /* Not Used */
{  48, /*                                           CH_DIGIT_ZERO, */  48,}, /* Not Used */
{  49, /*                                            CH_DIGIT_ONE, */  49,}, /* Not Used */
{  50, /*                                            CH_DIGIT_TWO, */  50,}, /* Not Used */
{  51, /*                                          CH_DIGIT_THREE, */  51,}, /* Not Used */
{  52, /*                                           CH_DIGIT_FOUR, */  52,}, /* Not Used */
{  53, /*                                           CH_DIGIT_FIVE, */  53,}, /* Not Used */
{  54, /*                                            CH_DIGIT_SIX, */  54,}, /* Not Used */
{  55, /*                                          CH_DIGIT_SEVEN, */  55,}, /* Not Used */
{  56, /*                                          CH_DIGIT_EIGHT, */  56,}, /* Not Used */
{  57, /*                                           CH_DIGIT_NINE, */  57,}, /* Not Used */
{  58, /*                                                CH_COLON, */  58,}, /* Not Used */
{  59, /*                                            CH_SEMICOLON, */  59,}, /* Not Used */
{  60, /*                                       CH_LESS_THAN_SIGN, */  60,}, /* Not Used */
{  61, /*                                          CH_EQUALS_SIGN, */  61,}, /* Not Used */
{  62, /*                                    CH_GREATER_THAN_SIGN, */  62,}, /* Not Used */
{  63, /*                                        CH_QUESTION_MARK, */  63,}, /* Not Used */
{  64, /*                                        CH_COMMERCIAL_AT, */  64,}, /* Not Used */
{  65, /*                               CH_LATIN_CAPITAL_LETTER_A, */  65,}, /* Not Used */
{  66, /*                               CH_LATIN_CAPITAL_LETTER_B, */  66,}, /* Not Used */
{  67, /*                               CH_LATIN_CAPITAL_LETTER_C, */  67,}, /* Not Used */
{  68, /*                               CH_LATIN_CAPITAL_LETTER_D, */  68,}, /* Not Used */
{  69, /*                               CH_LATIN_CAPITAL_LETTER_E, */  69,}, /* Not Used */
{  70, /*                               CH_LATIN_CAPITAL_LETTER_F, */  70,}, /* Not Used */
{  71, /*                               CH_LATIN_CAPITAL_LETTER_G, */  71,}, /* Not Used */
{  72, /*                               CH_LATIN_CAPITAL_LETTER_H, */  72,}, /* Not Used */
{  73, /*                               CH_LATIN_CAPITAL_LETTER_I, */  73,}, /* Not Used */
{  74, /*                               CH_LATIN_CAPITAL_LETTER_J, */  74,}, /* Not Used */
{  75, /*                               CH_LATIN_CAPITAL_LETTER_K, */  75,}, /* Not Used */
{  76, /*                               CH_LATIN_CAPITAL_LETTER_L, */  76,}, /* Not Used */
{  77, /*                               CH_LATIN_CAPITAL_LETTER_M, */  77,}, /* Not Used */
{  78, /*                               CH_LATIN_CAPITAL_LETTER_N, */  78,}, /* Not Used */
{  79, /*                               CH_LATIN_CAPITAL_LETTER_O, */  79,}, /* Not Used */
{  80, /*                               CH_LATIN_CAPITAL_LETTER_P, */  80,}, /* Not Used */
{  81, /*                               CH_LATIN_CAPITAL_LETTER_Q, */  81,}, /* Not Used */
{  82, /*                               CH_LATIN_CAPITAL_LETTER_R, */  82,}, /* Not Used */
{  83, /*                               CH_LATIN_CAPITAL_LETTER_S, */  83,}, /* Not Used */
{  84, /*                               CH_LATIN_CAPITAL_LETTER_T, */  84,}, /* Not Used */
{  85, /*                               CH_LATIN_CAPITAL_LETTER_U, */  85,}, /* Not Used */
{  86, /*                               CH_LATIN_CAPITAL_LETTER_V, */  86,}, /* Not Used */
{  87, /*                               CH_LATIN_CAPITAL_LETTER_W, */  87,}, /* Not Used */
{  88, /*                               CH_LATIN_CAPITAL_LETTER_X, */  88,}, /* Not Used */
{  89, /*                               CH_LATIN_CAPITAL_LETTER_Y, */  89,}, /* Not Used */
{  90, /*                               CH_LATIN_CAPITAL_LETTER_Z, */  90,}, /* Not Used */
{  91, /*                                  CH_LEFT_SQUARE_BRACKET, */  91,}, /* Not Used */
{  92, /*                                      CH_REVERSE_SOLIDUS, */  92,}, /* Not Used */
{  93, /*                                 CH_RIGHT_SQUARE_BRACKET, */  93,}, /* Not Used */
{  94, /*                                    CH_CIRCUMFLEX_ACCENT, */  94,}, /* Not Used */
{  95, /*                                             CH_LOW_LINE, */  95,}, /* Not Used */
{  96, /*                                         CH_GRAVE_ACCENT, */  96,}, /* Not Used */
{  97, /*                                 CH_LATIN_SMALL_LETTER_A, */  97,}, /* Not Used */
{  98, /*                                 CH_LATIN_SMALL_LETTER_B, */  98,}, /* Not Used */
{  99, /*                                 CH_LATIN_SMALL_LETTER_C, */  99,}, /* Not Used */
{ 100, /*                                 CH_LATIN_SMALL_LETTER_D, */ 100,}, /* Not Used */
{ 101, /*                                 CH_LATIN_SMALL_LETTER_E, */ 101,}, /* Not Used */
{ 102, /*                                 CH_LATIN_SMALL_LETTER_F, */ 102,}, /* Not Used */
{ 103, /*                                 CH_LATIN_SMALL_LETTER_G, */ 103,}, /* Not Used */
{ 104, /*                                 CH_LATIN_SMALL_LETTER_H, */ 104,}, /* Not Used */
{ 105, /*                                 CH_LATIN_SMALL_LETTER_I, */ 105,}, /* Not Used */
{ 106, /*                                 CH_LATIN_SMALL_LETTER_J, */ 106,}, /* Not Used */
{ 107, /*                                 CH_LATIN_SMALL_LETTER_K, */ 107,}, /* Not Used */
{ 108, /*                                 CH_LATIN_SMALL_LETTER_L, */ 108,}, /* Not Used */
{ 109, /*                                 CH_LATIN_SMALL_LETTER_M, */ 109,}, /* Not Used */
{ 110, /*                                 CH_LATIN_SMALL_LETTER_N, */ 110,}, /* Not Used */
{ 111, /*                                 CH_LATIN_SMALL_LETTER_O, */ 111,}, /* Not Used */
{ 112, /*                                 CH_LATIN_SMALL_LETTER_P, */ 112,}, /* Not Used */
{ 113, /*                                 CH_LATIN_SMALL_LETTER_Q, */ 113,}, /* Not Used */
{ 114, /*                                 CH_LATIN_SMALL_LETTER_R, */ 114,}, /* Not Used */
{ 115, /*                                 CH_LATIN_SMALL_LETTER_S, */ 115,}, /* Not Used */
{ 116, /*                                 CH_LATIN_SMALL_LETTER_T, */ 116,}, /* Not Used */
{ 117, /*                                 CH_LATIN_SMALL_LETTER_U, */ 117,}, /* Not Used */
{ 118, /*                                 CH_LATIN_SMALL_LETTER_V, */ 118,}, /* Not Used */
{ 119, /*                                 CH_LATIN_SMALL_LETTER_W, */ 119,}, /* Not Used */
{ 120, /*                                 CH_LATIN_SMALL_LETTER_X, */ 120,}, /* Not Used */
{ 121, /*                                 CH_LATIN_SMALL_LETTER_Y, */ 121,}, /* Not Used */
{ 122, /*                                 CH_LATIN_SMALL_LETTER_Z, */ 122,}, /* Not Used */
{ 123, /*                                   CH_LEFT_CURLY_BRACKET, */ 123,}, /* Not Used */
{ 124, /*                                        CH_VERTICAL_LINE, */ 124,}, /* Not Used */
{ 125, /*                                  CH_RIGHT_CURLY_BRACKET, */ 125,}, /* Not Used */
{ 126, /*                                                CH_TILDE, */ 126,}, /* Not Used */
{ 127, /*                                               CH_DELETE, */ 127,}, /* Not Used */
{ 128, /*                  CH_LATIN_CAPITAL_LETTER_C_WITH_CEDILLA, */ 128,}, /* Not Used */
{ 129, /*                  CH_LATIN_SMALL_LETTER_U_WITH_DIAERESIS, */ 129,}, /* Not Used */
{ 130, /*                      CH_LATIN_SMALL_LETTER_E_WITH_ACUTE, */ 130,}, /* Not Used */
{ 131, /*                 CH_LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX, */ 131,}, /* Not Used */
{ 132, /*                  CH_LATIN_SMALL_LETTER_A_WITH_DIAERESIS, */ 132,}, /* Not Used */
{ 133, /*                      CH_LATIN_SMALL_LETTER_A_WITH_GRAVE, */ 133,}, /* Not Used */
{ 134, /*                 CH_LATIN_SMALL_LETTER_A_WITH_RING_ABOVE, */ 134,}, /* Not Used */
{ 135, /*                    CH_LATIN_SMALL_LETTER_C_WITH_CEDILLA, */ 135,}, /* Not Used */
{ 136, /*                 CH_LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX, */ 136,}, /* Not Used */
{ 137, /*                  CH_LATIN_SMALL_LETTER_E_WITH_DIAERESIS, */ 137,}, /* Not Used */
{ 138, /*                      CH_LATIN_SMALL_LETTER_E_WITH_GRAVE, */ 138,}, /* Not Used */
{ 139, /*                  CH_LATIN_SMALL_LETTER_I_WITH_DIAERESIS, */ 139,}, /* Not Used */
{ 140, /*                 CH_LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX, */ 140,}, /* Not Used */
{ 141, /*                      CH_LATIN_SMALL_LETTER_I_WITH_GRAVE, */ 141,}, /* Not Used */
{ 142, /*                CH_LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS, */ 142,}, /* Not Used */
{ 143, /*               CH_LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE, */ 143,}, /* Not Used */
{ 144, /*                    CH_LATIN_CAPITAL_LETTER_E_WITH_ACUTE, */ 144,}, /* Not Used */
{ 145, /*                              CH_LATIN_SMALL_LIGATURE_AE, */ 145,}, /* Not Used */
{ 146, /*                            CH_LATIN_CAPITAL_LIGATURE_AE, */ 146,}, /* Not Used */
{ 147, /*                 CH_LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX, */ 147,}, /* Not Used */
{ 148, /*                  CH_LATIN_SMALL_LETTER_O_WITH_DIAERESIS, */ 148,}, /* Not Used */
{ 149, /*                      CH_LATIN_SMALL_LETTER_O_WITH_GRAVE, */ 149,}, /* Not Used */
{ 150, /*                 CH_LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX, */ 150,}, /* Not Used */
{ 151, /*                      CH_LATIN_SMALL_LETTER_U_WITH_GRAVE, */ 151,}, /* Not Used */
{ 152, /*                  CH_LATIN_SMALL_LETTER_Y_WITH_DIAERESIS, */ 152,}, /* Not Used */
{ 153, /*                CH_LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS, */ 153,}, /* Not Used */
{ 154, /*                CH_LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS, */ 154,}, /* Not Used */
{ 155, /*                                            CH_CENT_SIGN, */ 155,}, /* Not Used */
{ 156, /*                                           CH_POUND_SIGN, */ 156,}, /* Not Used */
{                                                     CH_YEN_SIGN,    157,},
{ 158, /*                                          CH_PESETA_SIGN, */ 158,}, /* Not Used */
{ 159, /*                       CH_LATIN_SMALL_LETTER_F_WITH_HOOK, */ 159,}, /* Not Used */
{ 160, /*                      CH_LATIN_SMALL_LETTER_A_WITH_ACUTE, */ 160,}, /* Not Used */
{ 161, /*                      CH_LATIN_SMALL_LETTER_I_WITH_ACUTE, */ 161,}, /* Not Used */
{ 162, /*                      CH_LATIN_SMALL_LETTER_O_WITH_ACUTE, */ 162,}, /* Not Used */
{ 163, /*                      CH_LATIN_SMALL_LETTER_U_WITH_ACUTE, */ 163,}, /* Not Used */
{ 164, /*                      CH_LATIN_SMALL_LETTER_N_WITH_TILDE, */ 164,}, /* Not Used */
{ 165, /*                    CH_LATIN_CAPITAL_LETTER_N_WITH_TILDE, */ 165,}, /* Not Used */
{ 166, /*                           CH_FEMININE_ORDINAL_INDICATOR, */ 166,}, /* Not Used */
{ 167, /*                          CH_MASCULINE_ORDINAL_INDICATOR, */ 167,}, /* Not Used */
{ 168, /*                               CH_INVERTED_QUESTION_MARK, */ 168,}, /* Not Used */
{ 169, /*                                    CH_REVERSED_NOT_SIGN, */ 169,}, /* Not Used */
{ 170, /*                                             CH_NOT_SIGN, */ 170,}, /* Not Used */
{ 171, /*                             CH_VULGAR_FRACTION_ONE_HALF, */ 171,}, /* Not Used */
{ 172, /*                          CH_VULGAR_FRACTION_ONE_QUARTER, */ 172,}, /* Not Used */
{ 173, /*                            CH_INVERTED_EXCLAMATION_MARK, */ 173,}, /* Not Used */
{                    CH_LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK,    174,},
{                   CH_RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK,    175,},
{                                                  CH_LIGHT_SHADE,    176,},
{                                                 CH_MEDIUM_SHADE,    177,},
{                                                   CH_DARK_SHADE,    178,},
{                                  CH_BOX_DRAWINGS_LIGHT_VERTICAL,    179,},
{                         CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_LEFT,    180,},
{                 CH_BOX_DRAWINGS_VERTICAL_SINGLE_AND_LEFT_DOUBLE,    181,},
{                 CH_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_LEFT_SINGLE,    182,},
{                     CH_BOX_DRAWINGS_DOWN_DOUBLE_AND_LEFT_SINGLE,    183,},
{                     CH_BOX_DRAWINGS_DOWN_SINGLE_AND_LEFT_DOUBLE,    184,},
{ 185, /*                CH_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_LEFT, */ 185,}, /* Not Used */
{                                 CH_BOX_DRAWINGS_DOUBLE_VERTICAL,    186,},
{ 187, /*                    CH_BOX_DRAWINGS_DOUBLE_DOWN_AND_LEFT, */ 187,}, /* Not Used */
{ 188, /*                      CH_BOX_DRAWINGS_DOUBLE_UP_AND_LEFT, */ 188,}, /* Not Used */
{ 189, /*               CH_BOX_DRAWINGS_UP_DOUBLE_AND_LEFT_SINGLE, */ 189,}, /* Not Used */
{                       CH_BOX_DRAWINGS_UP_SINGLE_AND_LEFT_DOUBLE,    190,},
{                             CH_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT,    191,},
{                              CH_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT,    192,},
{ 193, /*                 CH_BOX_DRAWINGS_LIGHT_UP_AND_HORIZONTAL, */ 193,}, /* Not Used */
{ 194, /*               CH_BOX_DRAWINGS_LIGHT_DOWN_AND_HORIZONTAL, */ 194,}, /* Not Used */
{                        CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_RIGHT,    195,},
{                                CH_BOX_DRAWINGS_LIGHT_HORIZONTAL,    196,},
{                   CH_BOX_DRAWINGS_LIGHT_VERTICAL_AND_HORIZONTAL,    197,},
{                CH_BOX_DRAWINGS_VERTICAL_SINGLE_AND_RIGHT_DOUBLE,    198,},
{                CH_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_RIGHT_SINGLE,    199,},
{ 200, /*                     CH_BOX_DRAWINGS_DOUBLE_UP_AND_RIGHT, */ 200,}, /* Not Used */
{ 201, /*                   CH_BOX_DRAWINGS_DOUBLE_DOWN_AND_RIGHT, */ 201,}, /* Not Used */
{ 202, /*                CH_BOX_DRAWINGS_DOUBLE_UP_AND_HORIZONTAL, */ 202,}, /* Not Used */
{                      CH_BOX_DRAWINGS_DOUBLE_DOWN_AND_HORIZONTAL,    203,},
{ 204, /*               CH_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_RIGHT, */ 204,}, /* Not Used */
{                               CH_BOX_DRAWINGS_DOUBLE_HORIZONTAL,    205,},
{ 206, /*          CH_BOX_DRAWINGS_DOUBLE_VERTICAL_AND_HORIZONTAL, */ 206,}, /* Not Used */
{ 207, /*         CH_BOX_DRAWINGS_UP_SINGLE_AND_HORIZONTAL_DOUBLE, */ 207,}, /* Not Used */
{ 208, /*         CH_BOX_DRAWINGS_UP_DOUBLE_AND_HORIZONTAL_SINGLE, */ 208,}, /* Not Used */
{               CH_BOX_DRAWINGS_DOWN_SINGLE_AND_HORIZONTAL_DOUBLE,    209,},
{               CH_BOX_DRAWINGS_DOWN_DOUBLE_AND_HORIZONTAL_SINGLE,    210,},
{ 211, /*              CH_BOX_DRAWINGS_UP_DOUBLE_AND_RIGHT_SINGLE, */ 211,}, /* Not Used */
{ 212, /*              CH_BOX_DRAWINGS_UP_SINGLE_AND_RIGHT_DOUBLE, */ 212,}, /* Not Used */
{ 213, /*            CH_BOX_DRAWINGS_DOWN_SINGLE_AND_RIGHT_DOUBLE, */ 213,}, /* Not Used */
{ 214, /*            CH_BOX_DRAWINGS_DOWN_DOUBLE_AND_RIGHT_SINGLE, */ 214,}, /* Not Used */
{ 215, /*   CH_BOX_DRAWINGS_VERTICAL_DOUBLE_AND_HORIZONTAL_SINGLE, */ 215,}, /* Not Used */
{ 216, /*   CH_BOX_DRAWINGS_VERTICAL_SINGLE_AND_HORIZONTAL_DOUBLE, */ 216,}, /* Not Used */
{                               CH_BOX_DRAWINGS_LIGHT_UP_AND_LEFT,    217,},
{                            CH_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT,    218,},
{                                                   CH_FULL_BLOCK,    219,},
{                                             CH_LOWER_HALF_BLOCK,    220,},
{                                              CH_LEFT_HALF_BLOCK,    221,},
{                                             CH_RIGHT_HALF_BLOCK,    222,},
{                                             CH_UPPER_HALF_BLOCK,    223,},
{ 224, /*                             CH_GREEK_SMALL_LETTER_ALPHA, */ 224,}, /* Not Used */
{                                   CH_LATIN_SMALL_LETTER_SHARP_S,    225,},
{ 226, /*                           CH_GREEK_CAPITAL_LETTER_GAMMA, */ 226,}, /* Not Used */
{ 227, /*                                CH_GREEK_SMALL_LETTER_PI, */ 227,}, /* Not Used */
{                                   CH_GREEK_CAPITAL_LETTER_SIGMA,    228,},
{ 229, /*                             CH_GREEK_SMALL_LETTER_SIGMA, */ 229,}, /* Not Used */
{ 230, /*                                           CH_MICRO_SIGN, */ 230,}, /* Not Used */
{ 231, /*                               CH_GREEK_SMALL_LETTER_TAU, */ 231,}, /* Not Used */
{                                     CH_GREEK_CAPITAL_LETTER_PHI,    232,},
{ 233, /*                           CH_GREEK_CAPITAL_LETTER_THETA, */ 233,}, /* Not Used */
{ 234, /*                           CH_GREEK_CAPITAL_LETTER_OMEGA, */ 234,}, /* Not Used */
{ 235, /*                             CH_GREEK_SMALL_LETTER_DELTA, */ 235,}, /* Not Used */
{                                                     CH_INFINITY,    236,},
{ 237, /*                               CH_GREEK_SMALL_LETTER_PHI, */ 237,}, /* Not Used */
{ 238, /*                           CH_GREEK_SMALL_LETTER_EPSILON, */ 238,}, /* Not Used */
{ 239, /*                                         CH_INTERSECTION, */ 239,}, /* Not Used */
{                                                 CH_IDENTICAL_TO,    240,},
{ 241, /*                                      CH_PLUS_MINUS_SIGN, */ 241,}, /* Not Used */
{ 242, /*                             CH_GREATER_THAN_OR_EQUAL_TO, */ 242,}, /* Not Used */
{ 243, /*                                CH_LESS_THAN_OR_EQUAL_TO, */ 243,}, /* Not Used */
{ 244, /*                                    CH_TOP_HALF_INTEGRAL, */ 244,}, /* Not Used */
{ 245, /*                                 CH_BOTTOM_HALF_INTEGRAL, */ 245,}, /* Not Used */
{ 246, /*                                        CH_DIVISION_SIGN, */ 246,}, /* Not Used */
{                                              CH_ALMOST_EQUAL_TO,    247,},
{                                                  CH_DEGREE_SIGN,    248,},
{                                              CH_BULLET_OPERATOR,    249,},
{ 250, /*                                           CH_MIDDLE_DOT, */ 250,}, /* Not Used */
{ 251, /*                                          CH_SQUARE_ROOT, */ 251,}, /* Not Used */
{ 252, /*                     CH_SUPERSCRIPT_LATIN_SMALL_LETTER_N, */ 252,}, /* Not Used */
{ 253, /*                                      CH_SUPERSCRIPT_TWO, */ 253,}, /* Not Used */
{ 254, /*                                         CH_BLACK_SQUARE, */ 254,}, /* Not Used */
{ 255, /*                                       CH_NO_BREAK_SPACE, */ 255,}, /* Not Used */
};
  
#else
extern struct graphicschar *gchar;
#endif

#define CH_MAXIMUM 256

int translateGraphicsChar(int c);
