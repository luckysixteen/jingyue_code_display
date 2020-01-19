//
//  LZW.h
//  LZW
//
//  Created by 景玥 on 14-12-17.
//  Copyright (c) 2014年 Jen. All rights reserved.
//

/* select dictionary size */
#if !defined(BITS)
#define BITS 12
#endif
#if BITS == 12
/* Critical sizes for LZW */
#define PRESET_CODE_MAX 256
#define END_OF_INPUT 256
#define NEW_DICTIONARY 257
#define STARTING_CODE 258
#define MAX_CODE	4096
#define UNUSED_CODE 4096
#define DICTIONARY_SIZE 5021

/* all of these should be unsigned */
typedef unsigned short CodeType; /* can hold MAX_CODE */
typedef unsigned short IndexType; /* can hold DICTIONARY_SIZE */
typedef unsigned long CountType; /* used for statistics only */
#define SIGNATURE "LZW12"


#elif BITS == 16 /* requires a /* Critical sizes for LZW */
#define PRESET_CODE_MAX 256
#define END_OF_INPUT 256
#define NEW_DICTIONARY 257
#define UNUSED_CODE 258
#define STARTING_CODE 259
#define MAX_CODE	65536
#define DICTIONARY_SIZE 81901L
typedef unsigned short CodeType;
typedef unsigned long IndexType;
typedef unsigned long CountType;
/* 32-bit context */
/* codes like this are preset */ /* this code terminates input */ /* reinitialize the dictionary */ /* an invalid code */
/*	first code we can use */
/*	2 * BITS */
/*	a prime ? > MAX CODE * 1.2 */
/* can hold MAX_CODE */ /* can hold DICTIONARY_SIZE */ /* used for statistics only */


#define SIGNATURE "LZW16"
#else
#error Undefined BITS value!
#endif
