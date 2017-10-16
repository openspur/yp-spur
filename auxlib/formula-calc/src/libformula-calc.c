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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <sys/time.h>

#include <formula-calc.h>
#include <formula.h>

double math_b_and( double **val )
{
	return (*val[0] > 0) && (*val[1] > 0);
}

double math_b_or( double **val )
{
	return (*val[0] > 0) || (*val[1] > 0);
}

double math_b_not( double **val )
{
	return !(*val[0] > 0);
}

double math_eq( double **val )
{
	return *val[0] == *val[1];
}

double math_neq( double **val )
{
	return *val[0] != *val[1];
}

double math_gtr( double **val )
{
	return *val[0] > *val[1];
}

double math_geq( double **val )
{
	return *val[0] >= *val[1];
}

double math_lss( double **val )
{
	return *val[0] < *val[1];
}

double math_leq( double **val )
{
	return *val[0] <= *val[1];
}

double math_add( double **val )
{
	return *val[0] + *val[1];
}

double math_sub( double **val )
{
	return *val[0] - *val[1];
}

double math_mul( double **val )
{
	return *val[0] * *val[1];
}

double math_div( double **val )
{
	return *val[0] / *val[1];
}

double math_pow( double **val )
{
	return pow( *val[0], *val[1] );
}

double math_sin( double **val )
{
	return sin( *val[0] );
}

double math_cos( double **val )
{
	return cos( *val[0] );
}

double math_tan( double **val )
{
	return tan( *val[0] );
}

double math_sinh( double **val )
{
	return sinh( *val[0] );
}

double math_cosh( double **val )
{
	return cosh( *val[0] );
}

double math_tanh( double **val )
{
	return tanh( *val[0] );
}

double math_atan( double **val )
{
	return atan( *val[0] );
}

double math_asin( double **val )
{
	return asin( *val[0] );
}

double math_acos( double **val )
{
	return acos( *val[0] );
}

double math_atan2( double **val )
{
	return atan2( *val[0], *val[1] );
}

double math_sqrt( double **val )
{
	return sqrt( *val[0] );
}

double math_exp( double **val )
{
	return exp( *val[0] );
}

double math_let( double **val )
{
	return *val[0] = *val[1];
}

double math_sign( double **val )
{
	if( *val[0] < 0 ) return -1;
	return 1;
}

double math_fabs( double **val )
{
	return fabs( *val[0] );
}

double math_log10( double **val )
{
	return log10( *val[0] );
}

double math_ln( double **val )
{
	return log( *val[0] );
}

double math_pi( double **val )
{
	return M_PI;
}

double math_e( double **val )
{
	return M_E;
}

double math_mod( double **val )
{
	return fmod( *val[0], *val[1] );
}

double math_round( double **val )
{
	return round( *val[0] );
}

#define OPERATION_MUL 11
struct operation_t operation[] =
{
	{ "==",     4, TYPE_OP,   math_eq,    2 },
	{ "=",      1, TYPE_OP,   math_let,   2 },
	{ "||",     2, TYPE_OP,   math_b_or,  2 },
	{ "&&",     3, TYPE_OP,   math_b_and, 2 },
	{ "!=",     4, TYPE_OP,   math_neq,   2 },
	{ "<=",     5, TYPE_OP,   math_leq,   2 },
	{ ">=",     5, TYPE_OP,   math_geq,   2 },
	{ "<",      5, TYPE_OP,   math_lss,   2 },
	{ ">",      5, TYPE_OP,   math_gtr,   2 },
	{ "+",      7, TYPE_OP,   math_add,   2 },
	{ "-",      7, TYPE_OP,   math_sub,   2 },
	{ "*",      8, TYPE_OP,   math_mul,   2 },
	{ "/",      8, TYPE_OP,   math_div,   2 },
	{ "!",     11, TYPE_OP,   math_b_not, 1 },
	{ "pi",    17, TYPE_MATH, math_pi,    0 },
	{ "e",     17, TYPE_MATH, math_e,     0 },
	{ "log10", 17, TYPE_MATH, math_log10, 1 },
	{ "ln",    17, TYPE_MATH, math_ln,    1 },
	{ "sin",   17, TYPE_MATH, math_sin,   1 },
	{ "cos",   17, TYPE_MATH, math_cos,   1 },
	{ "tan",   17, TYPE_MATH, math_tan,   1 },
	{ "sinh",  17, TYPE_MATH, math_sinh,  1 },
	{ "cosh",  17, TYPE_MATH, math_cosh,  1 },
	{ "tanh",  17, TYPE_MATH, math_tanh,  1 },
	{ "asin",  17, TYPE_MATH, math_asin,  1 },
	{ "acos",  17, TYPE_MATH, math_acos,  1 },
	{ "atan2", 17, TYPE_MATH, math_atan2, 2 },
	{ "atan",  17, TYPE_MATH, math_atan,  1 },
	{ "exp",   17, TYPE_MATH, math_exp,   1 },
	{ "sqrt",  17, TYPE_MATH, math_sqrt,  1 },
	{ "abs",   17, TYPE_MATH, math_fabs,  1 },
	{ "sign",  17, TYPE_MATH, math_sign,  1 },
	{ "round", 17, TYPE_MATH, math_round, 1 },
	{ "mod",   17, TYPE_MATH, math_mod,   2 },
	{ "pow",   17, TYPE_MATH, math_pow,   2 },
	{ "",     -1, TYPE_MAX , NULL,       0 }
};

struct rpf_t *rpf_last( struct rpf_t *rpf )
{
	for( ; rpf->next; rpf = rpf->next );
	
	return rpf;
}

struct rpf_t *rpf_push( struct rpf_t *rpf, struct stack_t *obj )
{
	rpf->next = malloc( sizeof( struct rpf_t ) );
	rpf->next->type = obj->type;
	rpf->next->value = obj->value;
	rpf->next->next = NULL;
	
	return rpf->next;
}

struct rpf_t *rpf_join( struct rpf_t *rpf, struct rpf_t *rpf2 )
{
	rpf->next = rpf2;
	for( ; rpf->next; rpf = rpf->next );
	
	return rpf;
}

int rpf_count_num( struct rpf_t *rpf )
{
	int count;
	
	count = 0;
	for( ; rpf; rpf = rpf->next )
	{
		count ++;
	}
	
	return count;
}

struct rpf_t *formula_output( struct stack_t *num, int *sp_num, struct stack_t *op, int *sp_op, int rank )
{
	/*{
		int i;
		printf("  op ");
		for(i = 0; i < *sp_op; i++)
			printf("%s ", ((struct operation_t*)op[i].value)->op);
		printf("\n  num ");
		for(i = 0; i < *sp_num; i++)
		{
			if(num[i].type == TYPE_RPF)
				printf("rpf ");
			else
				printf("%f ", *((double*)num[i].value));
		}
		printf("\n");
	}*/
	
	int sp_op2;
	for( sp_op2 = *sp_op - 1; sp_op2 >= 0; sp_op2 --)
	{
		struct rpf_t rpf2, *rpf2_tmp;
		int sp_num2;

		rpf2_tmp = &rpf2;
		rpf2.type = TYPE_START;

		int num_cnt;
		num_cnt = ((struct operation_t*)op[sp_op2].value)->narg;
		if( num_cnt > *sp_num ) return NULL;
		for( sp_num2 = *sp_num - num_cnt; sp_num2 < *sp_num; sp_num2 ++)
		{
			switch( num[sp_num2].type )
			{
			case TYPE_RPF:
				rpf2_tmp = rpf_join( rpf2_tmp, num[sp_num2].value );
				//printf(" rpf");
				break;
			default:
				rpf2_tmp = rpf_push( rpf2_tmp, &num[sp_num2] );
				//printf("%f ", *((double*)num[sp_num2].value));
				break;
			}
		}
		//printf("\n");
		(*sp_num) -= num_cnt;
		rpf2_tmp = rpf_push( rpf2_tmp, &op[sp_op2] );
		(*sp_op) --;

		num[*sp_num].rank = 0;
		num[*sp_num].type = TYPE_RPF;
		num[*sp_num].value = rpf2.next;
		(*sp_num) ++;

		//formula_print(stdout, &rpf2);
		//printf("\n");
	}
	return num[*sp_num - 1].value;
}

int formula( const char *expr, struct rpf_t **rpf, const struct variables_t *variable )
{
	int i;
	struct stack_t op[ 256 ];
	struct stack_t num[ 256 ];
	int sp_op, sp_num;
	struct rpf_t rpf_start;
	struct rpf_t *rpf_tmp;
	*rpf = NULL;
	int op_cont;
	
	sp_num = 0;
	sp_op = 0;
	op_cont = 0;
	for( ; *expr; expr ++ )
	{
		//printf( "[o%d/n%d] %s\n", sp_op, sp_num, expr );
		if( *expr == '(' )
		{
			const char *end;
			int rank2;
			struct rpf_t *rpf2;
			
			rank2 = 0;
			for( ; isspace( *expr ); expr ++ );
			for( end = expr; *end; end ++ )
			{
				if( *end == '(' ) rank2 ++;
				if( *end == ')' ) rank2 --;
				if( rank2 == 0 ) break;
			}
			if( rank2 || !(*end) ) return 0;
			if( formula( expr + 1, &rpf2, variable ) == 0 ) return 0;
			if( rpf2 )
			{
				num[sp_num].rank = 0;
				num[sp_num].type = TYPE_RPF;
				num[sp_num].value = rpf2;
				sp_num ++;
			}
			
			expr = end;
			op_cont = 0;
			continue;
		}
		else if( *expr == ')' )
		{
			op_cont = 0;
			break;
		}
		else if( *expr == ',' )
		{
			if( formula_output( num, &sp_num, op, &sp_op, 0 ) == NULL )
				return 0;
			op_cont = 0;
			continue;
		}
		else if( isspace( *expr ) )
		{
			op_cont = 0;
			continue;
		}
		for( i = 0; operation[i].type != TYPE_MAX; i ++ )
		{
			if( strstr( expr, operation[i].op ) == expr)
			{
				if( sp_op > 0 && op[sp_op-1].rank == operation[i].rank )
				{
					int sp_op2;
					sp_op2 = 1;
					if( formula_output( num, &sp_num, op + sp_op - 1, &sp_op2, operation[i].rank ) == NULL )
						return 0;
					sp_op --;
				}
				else if( sp_op > 0 && op[sp_op-1].rank > operation[i].rank )
				{
					if( formula_output( num, &sp_num, op, &sp_op, operation[i].rank ) == NULL )
						return 0;
				}
				expr += strlen( operation[i].op );
				for( ; isspace( *expr ); expr ++ );
				
				if( sp_num == 0 && operation[i].func == math_sub )
				{
					// Case: start with -
					num[sp_num].rank = 0;
					num[sp_num].type = TYPE_VALUE;
					num[sp_num].value = malloc( sizeof(double) );
					*((double*)num[sp_num].value) = -1;
					sp_num ++;
					op[sp_op].rank = operation[OPERATION_MUL].rank;
					op[sp_op].type = operation[OPERATION_MUL].type;
					op[sp_op].value = &operation[OPERATION_MUL];
					sp_op ++;
					op_cont = 0;
					expr --;
					break;
				}
				else if( op_cont == 1 && operation[i].type != TYPE_MATH )
				{
					if( operation[i].func == math_sub )
					{
						// Case: x*-y
						num[sp_num].rank = 0;
						num[sp_num].type = TYPE_VALUE;
						num[sp_num].value = malloc( sizeof(double) );
						*((double*)num[sp_num].value) = -1;
						sp_num ++;
						op[sp_op].rank = operation[OPERATION_MUL].rank;
						op[sp_op].type = operation[OPERATION_MUL].type;
						op[sp_op].value = &operation[OPERATION_MUL];
						sp_op ++;
						op_cont = 0;
						expr --;
						break;
					}
					else
					{
						// Case: x*/y (Error)
						return 0;
					}
				}
				
				if( operation[i].type != TYPE_MATH )
				{
					op_cont = 1;
				}
				else
				{
					op[sp_op].rank = operation[i].rank;
					op[sp_op].type = operation[i].type;
					op[sp_op].value = &operation[i];
					sp_op ++;
					op_cont = 0;
					if( operation[i].narg > 0 )
					{
						const char *end;
						int rank2;
						struct rpf_t *rpf2;
						
						rank2 = 0;
						
						for( end = expr; *end; end ++ )
						{
							if( *end == '(' ) rank2 ++;
							if( *end == ')' ) rank2 --;
							if( rank2 == 0 ) break;
						}
						if( rank2 || !(*end) ) return 0;
						if( formula( expr + 1, &rpf2, variable ) == 0 ) return 0;
						if( !rpf2 ) return 0;
						
						num[sp_num].rank = 0;
						num[sp_num].type = TYPE_RPF;
						num[sp_num].value = rpf2;
						rpf_push( rpf_last( rpf2 ), &op[sp_op-1] );
						sp_op --;
						sp_num ++;
						expr = end + 1;
					}
				}
				if( operation[i].type != TYPE_MATH )
				{
					op[sp_op].rank = operation[i].rank;
					op[sp_op].type = operation[i].type;
					op[sp_op].value = &operation[i];
					sp_op ++;
				}
				expr --;

				break;
			}
		}
		if( operation[i].type != TYPE_MAX ) continue;
		op_cont = 0;
		if( isalpha( *expr ) )
		{
			const char *end;
			char variable_name[256];
			int i;
			
			if( !variable ) return 0;
			
			i = 0;
			for( end = expr; *end; end ++ )
			{
				if( !( isalnum( *end ) || *end == '_' || *end == '[' || *end == ']' ) ) break;
				variable_name[ i++ ] = *end;
			}
			variable_name[ i ] = 0;

			for( i = 0; variable[i].name; i ++ )
			{
				if( strcmp( variable[i].name, variable_name ) == 0 )
				{
					num[sp_num].rank = 0;
					num[sp_num].type = TYPE_VARIABLE;
					num[sp_num].value = variable[i].pointer;
					sp_num ++;
					break;
				}
			}
			if( !variable[i].name ) return 0;
			expr = end - 1;
		}
		else
		{
			const char *end;
			
			for( end = expr; *end; end ++ )
			{
				if( !( isdigit( *end ) || ( end == expr && *end == '-' ) || *end == '.' ) ) break;
			}
			num[sp_num].rank = 0;
			num[sp_num].type = TYPE_VALUE;
			num[sp_num].value = malloc( sizeof(double) );
			*((double*)num[sp_num].value) = strtod( expr, NULL );
			sp_num ++;
			expr = end - 1;
		}
	}
	if( formula_output( num, &sp_num, op, &sp_op, 0 ) == NULL )
		return 0;
	if( sp_num < 0 || sp_op != 0 ) return 0;
	
	rpf_start.type = TYPE_START;
	rpf_tmp = &rpf_start;
	for( sp_num --; sp_num >= 0; sp_num -- )
	{
		switch( num[sp_num].type )
		{
		case TYPE_RPF:
			rpf_tmp = rpf_join( rpf_tmp, num[sp_num].value );
			break;
		default:
			rpf_tmp = rpf_push( rpf_tmp, &num[sp_num] );
			break;
		}
	}
	*rpf = rpf_start.next;
	return 1;
}

void formula_free( struct rpf_t *rpf )
{
	for( ; rpf; )
	{
		struct rpf_t *tmp;
		switch( rpf->type )
		{
		case TYPE_VALUE:
			free( rpf->value );
			break;
		case TYPE_VARIABLE:
			break;
		default:
			break;
		}
		tmp = rpf->next;
		free( rpf );
		rpf = tmp;
	}
}

double formula_eval( struct rpf_t *rpf )
{
	double _stack[512];
	double *stack[512];
	int sp;
	struct operation_t *op;
	double tmp;
	
	sp = 0;
	for( ; rpf; rpf = rpf->next )
	{
		switch( rpf->type )
		{
		case TYPE_VALUE:
		case TYPE_VARIABLE:
			stack[sp] = ( (double*)rpf->value );
			sp ++;
			break;
		default:
			op = (struct operation_t*)rpf->value;
			tmp = (op->func)( &stack[ sp - op->narg ] );
			sp -= op->narg;
			stack[sp] = &_stack[sp];
			_stack[sp] = tmp;
			sp ++;
			break;
		}
	}
	return *stack[0];
}

struct rpf_t *formula_optimize( struct rpf_t *rpf_orig )
{
	double *stack[512];
	struct rpf_t *rpf_st[512];
	int fixed[512];
	int sp;
	struct rpf_t *rpf_new;
	struct rpf_t rpf_new_st;
	int i;
	struct rpf_t *rpf = rpf_orig;
	
	sp = 0;
	for( ; rpf; rpf = rpf->next )
	{
		struct operation_t *op;
		double tmp;
		switch( rpf->type )
		{
		case TYPE_VALUE:
		case TYPE_VARIABLE:
			fixed[sp] = 0;
			rpf_st[sp] = malloc(sizeof(struct rpf_t));
			*(rpf_st[sp]) = *rpf;
			if( rpf->type == TYPE_VALUE )
			{
				stack[sp] = malloc(sizeof(double));
				*(stack[sp]) =  *( (double*)rpf->value );
				rpf_st[sp]->value = stack[sp];
				fixed[sp] = 1;
			}
			else
			{
				stack[sp] = NULL;
				fixed[sp] = 0;
			}
			sp ++;
			break;
		default:
			op = (struct operation_t*)rpf->value;
			for( i = 0; i < op->narg; i ++ )
			{
				if( !fixed[sp-1-i] ) break;
			}
			if( i == op->narg )
			{
				tmp = (op->func)( &stack[ sp - op->narg ] );
				for( i = 0; i < op->narg; i ++ )
				{
					if( stack[sp-1-i] )
						free(stack[sp-1-i]);
					free(rpf_st[sp-1-i]);
				}
				sp -= op->narg;
				stack[sp] = malloc(sizeof(double));
				*(stack[sp]) = tmp;
				fixed[sp] = 1;
				
				rpf_st[sp] = malloc(sizeof(struct rpf_t));
				rpf_st[sp]->type = TYPE_VALUE;
				rpf_st[sp]->value = stack[sp];
				sp ++;
			}
			else
			{
				rpf_st[sp] = malloc(sizeof(struct rpf_t));
				*(rpf_st[sp]) = *rpf;
				fixed[sp] = 0;
				sp ++;
			}
			break;
		}
	}
	
	rpf_new = &rpf_new_st;
	rpf_new->type = TYPE_START;
	for( i = 0; i < sp; i ++ )
	{
		rpf_new->next = rpf_st[i];
		rpf_new->next->next = NULL;
		rpf_new = rpf_new->next;
	}
	return rpf_new_st.next;
}

void formula_print( FILE *stream, struct rpf_t *rpf )
{
	for( ; rpf; rpf = rpf->next )
	{
		switch( rpf->type )
		{
		case TYPE_VALUE:
			fprintf( stream, "%f ", *(double*)rpf->value );
			break;
		case TYPE_VARIABLE:
			fprintf( stream, "VARIABLE " );
			break;
		default:
			fprintf( stream, "%s ", ((struct operation_t*)rpf->value)->op );
			break;
		}
	}
}

