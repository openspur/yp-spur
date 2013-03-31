#ifndef __FILTER_H__
#define __FILTER_H__

typedef struct
{
	int mov_avg[24];
	int i;
} Filter1st;

int Filter1st_Filter( Filter1st *filter, int input );
int Filter1st_CreateLPF( Filter1st *filter );


#endif
