#include "condfilters.h"
#include "parser.h"
#include <malloc.h>

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

RowSelectionLinkedList run_filter(Table* table, ConditionFilter filter)
{
	RowSelectionLinkedList list;
	list.num_rows = 0;
	list.start = NULL;
	list.end = NULL;

	for (int i = 0; i < table->num_rows; ++i)
	{
		Row* row = table->rows[i];
		if (!filter.filter_func(row))
		{
			continue;
		}

		++list.num_rows;

		SelectedRow* srow = malloc(sizeof(SelectedRow));
		srow->ref = row;
		srow->next = NULL;

		if (list.end)
		{
			list.end->next = srow;
			list.end = srow;
		}
		else
		{
			list.start = srow;
			list.end = srow;
		}
	}

	return list;
}