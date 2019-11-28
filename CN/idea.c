#include <string.h>
#include "idea.h"

#ifdef IDEA32	/* Use >16-bit temporaries */
#define low16(x) ((x) & 0xFFFF)
typedef unsigned int uint16;	/* at LEAST 16 bits, maybe more */
#else
#define low16(x) (x)	/* this is only ever applied to uint16's */
typedef word16 uint16;
#endif

#ifdef _GNUC_
#define CONST __const__
#else
#define CONST
#endif

#ifdef SMALL_CACHE
CONST static uint16
mul(register uint16 a, register uint16 b)
{
	register word32 p;

	p = (word32)a * b;
	if (p) {
		b = low16(p);
		a = p>>16;
		return (b - a) + (b < a);
	} else if (a) {
		return 1-b;
	} else {
		return 1-a;
	}
} /* mul */
#endif /* SMALL_CACHE */

/*
 * Compute the multiplicative inverse of x, modulo 65537, using Euclid's
 * algorithm. It is unrolled twice to avoid swapping the registers each
 * iteration, and some subtracts of t have been changed to adds.
 */
CONST static uint16 multiply(uint16 x) {

	uint16 t0, t1;
	uint16 q, y;

	if (x <= 1)
		return x;	/* 0 and 1 are self-inverse */
	t1 = 0x10001L / x;	/* Since x >= 2, this fits into 16 bits */
	y = 0x10001L % x;
	if (y == 1)
		return low16(1-t1);
	t0 = 1;
	do {
		q = x / y;
		x = x % y;
		t0 += q * t1;
		if (x == 1)
			return t0;
		q = y / x;
		y = y % x;
		t1 += q * t0;
	} while (y != 1);
	return low16(1-t1);
} 

void ideaExpandKey(byte const *userkey, word16 *EK){

	int i,j;

	for (j=0; j<8; j++) {
		EK[j] = (userkey[0]<<8) + userkey[1];
		userkey += 2;
	}
	for (i=0; j < IDEAKEYLEN; j++) {
		i++;
		EK[i+7] = (EK[i & 7] << 9) | (EK[i+1 & 7] >> 7);
		EK += i & 8;
		i &= 7;
	}
} 

void ideaInvertKey(word16 const *EK, word16 DK[IDEAKEYLEN]){

	int i;
	uint16 t1, t2, t3;
	word16 temp[IDEAKEYLEN];
	word16 *p = temp + IDEAKEYLEN;

	t1 = multiply(*EK++);
	t2 = -*EK++;
	t3 = -*EK++;
	*--p = multiply(*EK++);
	*--p = t3;
	*--p = t2;
	*--p = t1;

	for (i = 0; i < IDEAROUNDS-1; i++) {
		t1 = *EK++;
		*--p = *EK++;
		*--p = t1;

		t1 = multiply(*EK++);
		t2 = -*EK++;
		t3 = -*EK++;
		*--p = multiply(*EK++);
		*--p = t2;
		*--p = t3;
		*--p = t1;
	}
	t1 = *EK++;
	*--p = *EK++;
	*--p = t1;

	t1 = multiply(*EK++);
	t2 = -*EK++;
	t3 = -*EK++;
	*--p = multiply(*EK++);
	*--p = t3;
	*--p = t2;
	*--p = t1;

	memcpy(DK, temp, sizeof(temp));
	burn(temp);
}

#ifdef SMALL_CACHE
#define MUL(x,y) (x = mul(low16(x),y))
#else /* !SMALL_CACHE */
#ifdef AVOID_JUMPS
#define MUL(x,y) (x = low16(x-1), t16 = low16((y)-1), \
		t32 = (word32)x*t16 + x + t16 + 1, x = low16(t32), \
		t16 = t32>>16, x = (x-t16) + (x<t16) )
#else /* !AVOID_JUMPS (default) */

#define MUL(x,y) \
	((t16 = (y)) ? \
		(x=low16(x)) ? \
			t32 = (word32)x*t16, \
			x = low16(t32), \
			t16 = t32>>16, \
			x = (x-t16)+(x<t16) \
		: \
			(x = 1-t16) \
	: \
		(x = 1-x))
#endif

#endif

void ideaCipher(byte const (inbuf[8]), byte (outbuf[8]), word16 const *key) {

	register uint16 x1, x2, x3, x4, s2, s3;
	word16 *in, *out;

#ifndef SMALL_CACHE
	register uint16 t16;	
	register word32 t32;
#endif

	int r = IDEAROUNDS;

	in = (word16 *)inbuf;
	x1 = *in++;  x2 = *in++;
	x3 = *in++;  x4 = *in;

	x1 = (x1>>8) | (x1<<8);
	x2 = (x2>>8) | (x2<<8);
	x3 = (x3>>8) | (x3<<8);
	x4 = (x4>>8) | (x4<<8);

	do {
		MUL(x1,*key++);
		x2 += *key++;
		x3 += *key++;
		MUL(x4, *key++);

		s3 = x3;
		x3 ^= x1;
		MUL(x3, *key++);
		s2 = x2;
		x2 ^= x4;
		x2 += x3;
		MUL(x2, *key++);
		x3 += x2;

		x1 ^= x2;  x4 ^= x3;

		x2 ^= s3;  x3 ^= s2;
	} while (--r);

	MUL(x1, *key++);
	x3 += *key++;
	x2 += *key++;
	MUL(x4, *key);

	out = (word16 *)outbuf;


	x1 = low16(x1);
	x2 = low16(x2);
	x3 = low16(x3);
	x4 = low16(x4);
	*out++ = (x1>>8) | (x1<<8);
	*out++ = (x3>>8) | (x3<<8);
	*out++ = (x2>>8) | (x2<<8);
	*out   = (x4>>8) | (x4<<8);

} 


#include <stdio.h>
#include <time.h>

#ifndef BLOCKS
#ifndef KBYTES
#define KBYTES 1024
#endif
#define BLOCKS (64*KBYTES)
#endif

int main(void) {	
	/* Tester */
	int i, j, k;
	byte userkey[16];
	word16 EK[IDEAKEYLEN], DK[IDEAKEYLEN];
	byte XX[8], YY[8], ZZ[8];
	clock_t start, end;
	long l;

	/* Make a sample user key for testing... */
	for(i=0; i<16; i++)
		userkey[i] = i+1;

	/* Compute encryption subkeys from user key... */
	ideaExpandKey(userkey, EK);
	printf("\nEncryption key subblocks: ");
	for (j=0; j<IDEAROUNDS+1; j++) {
		printf("\nround %d:   ", j+1);
		if (j < IDEAROUNDS)
			for(i=0; i<6; i++)
				printf(" %6u", EK[j*6+i]);
		else
			for(i=0; i<4; i++)
				printf(" %6u", EK[j*6+i]);
	}

	/* Compute decryption subkeys from encryption subkeys... */
	ideaInvertKey(EK, DK);
	printf("\nDecryption key subblocks: ");
	for (j=0; j<IDEAROUNDS+1; j++) {
		printf("\nround %d:   ", j+1);
		if (j < IDEAROUNDS)
			for(i=0; i<6; i++)
				printf(" %6u", DK[j*6+i]);
		else
			for(i=0; i<4; i++)
				printf(" %6u", DK[j*6+i]);
	}

	/* Make a sample plaintext pattern for testing... */
	for (k=0; k<8; k++)
		XX[k] = k;

	printf("\n Encrypting %d bytes (%ld blocks)...", BLOCKS*16, BLOCKS);
	fflush(stdout);
	start = clock();
	memcpy(YY, XX, 8);
	for (l = 0; l < BLOCKS; l++)
		ideaCipher(YY, YY, EK);	/* repeated encryption */
	memcpy(ZZ, YY, 8);
	for (l = 0; l < BLOCKS; l++)
		ideaCipher(ZZ, ZZ, DK);	/* repeated decryption */
	end = clock() - start;
	l = end  / (CLOCKS_PER_SEC/1000) + 1;
	i = l/1000;
	j = l%1000;
	l = (16 * BLOCKS * (CLOCKS_PER_SEC/1000)) / (end/1000);
	printf("%d.%03d seconds = %ld bytes per second\n", i, j, l);

	printf("\nX %3u  %3u  %3u  %3u  %3u  %3u  %3u %3u\n",
	  XX[0], XX[1],  XX[2], XX[3], XX[4], XX[5],  XX[6], XX[7]);
	printf("\nY %3u  %3u  %3u  %3u  %3u  %3u  %3u %3u\n",
	  YY[0], YY[1],  YY[2], YY[3], YY[4], YY[5],  YY[6], YY[7]);
	printf("\nZ %3u  %3u  %3u  %3u  %3u  %3u  %3u %3u\n",    
	  ZZ[0], ZZ[1],  ZZ[2], ZZ[3], ZZ[4], ZZ[5],  ZZ[6], ZZ[7]);

	/* Now decrypted ZZ should be same as original XX */
	for (k=0; k<8; k++)
		if (XX[k] != ZZ[k]) {
			printf("\n\07Error!  Noninvertable encryption.\n");
			exit(-1);	/* error exit */ 
		}

	return 0;	
