#include "condfilters.h"
#include "parser.h"

int _all(Row* _)
{
	return 1;
}

ConditionFilter condfilter_all()
{
	ConditionFilter filter;
	filter.filter_func = *_all;
	return filter;	
}