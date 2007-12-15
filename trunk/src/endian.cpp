// This file was created by Scott C. Walton

#include "endian.h"


/*
    These functions were created by Scott C. Walton.

    They were renamed by Chris Johnson.
*/


uint32 swap_endian_32(uint32 nts)
{
#ifdef BIG_ENDIAN
	uint32 sn;
	for (int32 i = 0; i < sizeof(nts); ++i) {
		sn <<= 8;
		uint32 temp = nts & 0x00ff << (i * 8);
		temp >>= i * 8;
		sn |= temp;
	}
	return sn;
#endif
	return nts;
}

uint16 swap_endian_16(uint16 nts)
{
#ifdef BIG_ENDIAN
	uint16 sn;
	for (int i = 0; i < sizeof(nts); ++i) {
		sn <<= 8;
		uint16 temp = nts & 0x00ff << (i * 8);
		temp >>= i * 8;
		sn |= temp;
	}
	return sn;
#endif
	return nts;
}
