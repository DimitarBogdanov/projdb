#include "condfilters.h"
#include "parser.h"
#include <malloc.h>
#include <string.h>

int _all(Row* _)
{
	return 1;
}

ConditionFilter condfilter_all()
{
	ConditionFilter filter;
	filter.filter_func = *_all;
	filter.col_name_split = NULL;
	return filter;	
}

ConditionFilter condfilter_cols(char* col_names_split)
{
	ConditionFilter filter;
	filter.filter_func = *_all;
	filter.col_name_split = col_names_split;
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

void prepend(char* s, const char* t)
{
    size_t len = strlen(t);
    memmove(s + len, s, strlen(s) + 1);
    memcpy(s, t, len);
}

int should_show_col(char* col_names_split, char* col_name)
{
	if (col_names_split == NULL)
	{
		return 1;
	}

	if (strcmp(col_names_split, col_name) == 0)
	{
		return 1;
	}

	int len_col_name = strlen(col_name);
	int len = strlen(col_names_split);

	for (int i = 0; i < len; ++i)
	{
		int delimited = i == 0 || col_names_split[i-1] == ',';

		if (!delimited)
		{
			continue;
		}

		int match = 1;
		for (int j = 0; j < len_col_name; ++j)
		{
			if (col_names_split[i+j] != col_name[j])
			{
				match = 0;
				break;
			}
		}

		if (!match)
		{
			continue;
		}

		delimited = i+len_col_name >= len
			|| col_names_split[i+len_col_name] == ',';

		if (delimited)
		{
			return 1;
		}
	}

	return 0;
}