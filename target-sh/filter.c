#include "filter.h"

int Filter1st_Filter( Filter1st *filter, int input )
{
	int i, x;

	filter->mov_avg[ filter->i ] = input;
	filter->i ++;
	if( filter->i >= 20 ) filter->i = 0;
	
	x = 0;
	for( i = 0; i < 20; i ++ )
	{
		x += filter->mov_avg[i];
	}
	return x / 20;
}

int Filter1st_CreateLPF( Filter1st *filter )
{
	int i;
	for( i = 0; i < 20; i ++ )
	{
		filter->mov_avg[i] = 0;
	}
	filter->i = 0;
	return 1;
}
