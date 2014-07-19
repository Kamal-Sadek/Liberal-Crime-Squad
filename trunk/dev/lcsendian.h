//This file was created by Scott C. Walton

#ifndef ENDIAN_H
#define ENDIAN_H

#include "compat.h"

// Swaps the byte order of an Unsigned Long Integer for Big Endian Machines
// If BIG_ENDIAN is defined (Usually by passing -D BIG_ENDIAN to gcc), returns
// sn (Switched Number), which is nts (number_to_switch) in reverse byte order.
// Otherwise it returns its argument.
// This function was renamed by Chris Johnson
uint32 swap_endian_32(uint32 number_to_switch);

// Swaps the byte order of a 16 bit integer.
uint16 swap_endian_16(uint16 number_to_switch);

#endif
