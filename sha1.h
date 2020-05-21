
//	***************************************************************
//	*  sha1.h : macros and functions for the hash function sha-1  *
//  *  Created by Ricardo Belinha                                 *
//	***************************************************************

// input - message or file of size <= 2^64 - 1 bits
// output - a hash digest (checksum) of length 160 bits or 5 32-bit words, written in hexa


#ifndef _SHA1_H
#define _SHA1_H

#define uint8 unsigned char
#define uint32 unsigned long

// circular shift of x by n positions to the left
#define S(x,n) ((x<<n)|((x&0XFFFFFFFF)>>(32-n)))

#define F0(B,C,D) (D^(B&(C^D)))
#define F1(B,C,D) (B^C^D)
#define F2(B,C,D) (B&C)|(B&D)|(C&D)
#define F3(B,C,D) (B^C^D)

// take four bytes from b+i | get a word(uint32) called n
#define GET_UINT32(n,b,i)                       \
{                                               \
    (n) = ( (uint32) (b)[(i)    ] << 24 )       \
        | ( (uint32) (b)[(i) + 1] << 16 )       \
        | ( (uint32) (b)[(i) + 2] <<  8 )       \
        | ( (uint32) (b)[(i) + 3]       );      \
}

// take one word of 32 bits put it into four bytes, starting at b+i
#define PUT_UINT32(n,b,i)                       \
{                                               \
    (b)[(i)    ] = (uint8) ( (n) >> 24 );       \
    (b)[(i) + 1] = (uint8) ( (n) >> 16 );       \
    (b)[(i) + 2] = (uint8) ( (n) >>  8 );       \
    (b)[(i) + 3] = (uint8) ( (n)       );       \
}

uint32 K[80];
uint32 H[5];
uint32 W[80];
uint32 A, B, C, D, E;
uint32 TEMP;
uint8 binmsg[10001000];

//	testing messages, last two come from fips 180-1

const char * msg[] =
{
	"abc",
	"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
};

#endif	// _SHA1_H

//	******************************************************************

