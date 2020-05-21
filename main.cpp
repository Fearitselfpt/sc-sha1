//	*********************************************************************
//	*  sha1.cpp : cpp source for the hash function sha-1 implemntation  *
//	*  the implementation is based on the specification doc fips 180-1  *
//  *  Created by Ricardo Belinha                                       *
//	*********************************************************************

#include "sha1.h"
#include <string.h>
#include <stdio.h>

//	==================================================================
//	methods to be implemented
//	------------------------------------------------------------------

void initHs() {
    // load the initial values for H's
    H[0] = 0x67452301;
    H[1] = 0xEFCDAB89;
    H[2] = 0x98BADCFE;
    H[3] = 0x10325476;
    H[4] = 0xC3D2E1F0;
}

//	------------------------------------------------------------------

void initKs() {
    // load the initial values for K's
    // K = 5A827999 ( 0 <= t <= 19)
    // Kt = 6ED9EBA1 (20 <= t <= 39)
    // Kt = 8F1BBCDC (40 <= t <= 59)
    // Kt = CA62C1D6 (60 <= t <= 79).

    for(int t = 0; t <= 19; t++)
        K[t] = 0x5A827999;

    for(int t = 20; t <= 39; t++)
        K[t] = 0x6ED9EBA1;

    for(int t = 40; t <= 59; t++)
        K[t] = 0x8F1BBCDC;

    for(int t = 60; t <= 79; t++)
        K[t] = 0xCA62C1D6;
}

//	------------------------------------------------------------------

int getMessage(int k) {
    // copy message k from msg[k] to binmsg
    int msgLen = (int) strlen(msg[k]);
    memcpy(binmsg, msg[k], msgLen);
    return msgLen;
}

//	------------------------------------------------------------------

int paddMessage(int messLen) {
    // pad the binmsg array according to the specification
    // return the number of blocks of the padded message

    int numbBlocks, numZeros, bitLen;

    numbBlocks = (messLen +8)/64 + 1;
    numZeros = 63 - (messLen + 8)%64;

    memset(binmsg+messLen, 0x80, 1);
    memset(binmsg+messLen + 1, 0, numZeros + 4);

    bitLen = messLen * 8;
    // take one word of 32 bits put it into four bytes, starting at b+i
    PUT_UINT32(bitLen, binmsg, 64*numbBlocks - 4);

    return numbBlocks;
}

//	------------------------------------------------------------------

void getWsfromM(int currentBlock) {
    // fill out the W[] from the current block
    for(int i = 0; i <= 16; i++)
    // take four bytes from b+i | get a word(uint32) called n
    GET_UINT32(W[i], binmsg + (64 * currentBlock), 4 * i);
}

//	------------------------------------------------------------------

void getAsfromHs() {
    // initialize A, B, C, D, E from H's
    A = H[0]; B = H[1]; C = H[2]; D = H[3]; E = H[4];
}

//	---------------------A---------------------------------------------

void displayDigest(uint32 H[]) {
    printf(" digest value - %9lx %9lx %9lx %9lx %9lx\n\n",
           H[0], H[1], H[2], H[3], H[4]);
}

//	==================================================================

int main(int argc, char* argv[]) {
    int i, j, k;
    int messLen;
    int numBlocks;

    // parse all 3 test messages
    for (k=0; k<3; k++) {
        initKs();
        initHs();

        if (k!=2) {
            messLen = getMessage(k);
        } else {
            // third message is much bigger
            memset((void *)(binmsg), 'a', 1000000);
            messLen = 1000000;
        }
        numBlocks = paddMessage(messLen);

        for (i=0; i<numBlocks; i++) {
            getWsfromM(i);

            for (j=16; j<80; j++) {
                W[j] = W[j-3]^W[j-8]^W[j-14]^W[j-16];
                W[j] = S(W[j], 1);
            }

            getAsfromHs();

            for (j=0; j<80; j++) {
                TEMP = S(A,5);
                int cit = j / 20;

                switch (cit) {
                    case 0:
                        TEMP += F0(B,C,D);
                        break;
                    case 1:
                        TEMP += F1(B,C,D);
                        break;
                    case 2:
                        TEMP += F2(B,C,D);
                        break;
                    case 3:
                        TEMP += F3(B,C,D);
                        break;
                }

                TEMP = TEMP + E + W[j] + K[j];

                E=D;
                D=C;
                // circular shift of x by n positions to the left
                C=S(B,30);
                B=A;
                A=TEMP;
//				printf("%9x %9x %9x %9x %9x\n", A, B, C, D, E);
            }

            H[0] = H[0] + A;
            H[1] = H[1] + B;
            H[2] = H[2] + C;
            H[3] = H[3] + D;
            H[4] = H[4] + E;
        }
        displayDigest(H);
    }
    return 0;
}

//	******************************************************************
