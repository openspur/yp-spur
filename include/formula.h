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

#ifndef __FORMULA_H__
#define __FORMULA_H__

#include "formula-calc.h"

struct stack_t
{
	int rank;
	enum type_t type;
	void *value;
};

struct operation_t
{
	char op[8];
	int rank;
	enum type_t type;
	double (*func)(double**);
	int narg;
};

struct rpf_t *rpf_push( struct rpf_t *rpf, struct stack_t *obj );
struct rpf_t *rpf_join( struct rpf_t *rpf, struct rpf_t *rpf2 );
int rpf_count_num( struct rpf_t *rpf );
struct rpf_t *rpf_last( struct rpf_t *rpf );
struct rpf_t *formula_output( struct stack_t *num, int *sp_num, struct stack_t *op, int *sp_op, int rank );

#endif
