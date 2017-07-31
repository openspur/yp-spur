// Simple mathematical formula processing library
//  Written by Atsushi Watanabe
//  Intelligent Robot Laboratory, University of Tsukuba
//
// Copyright 2011 Atsushi Watanabe, All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this 
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR 
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
// EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef __FORMULA_CALC_H__
#define __FORMULA_CALC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

struct rpf_t
{
	enum type_t
	{
		TYPE_VALUE,
		TYPE_MATH,
		TYPE_VARIABLE,
		TYPE_OP,
		TYPE_MAX,
		TYPE_RPF,
		TYPE_START
	} type;
	void *value;
	struct rpf_t *next;
};

struct variables_t
{
	const char *name;
	double *pointer;
};

int formula( const char * expr, struct rpf_t **rpf, const struct variables_t *variable );
void formula_free( struct rpf_t *rpf );
double formula_eval( struct rpf_t *rpf );
struct rpf_t *formula_optimize( struct rpf_t *rpf );
void formula_print( FILE *stream, struct rpf_t *rpf );

#ifdef __cplusplus
}
#endif
#endif
