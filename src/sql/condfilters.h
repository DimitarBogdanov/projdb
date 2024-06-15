#pragma once

#include "parser.h"
#include "../data/dbdata.h"

/*
 * condfilters.h
 * Contains filters for selection queries.
 */

ConditionFilter condfilter_all();
ConditionFilter condfilter_cols(char* col_names_split); // delimiter is comma

RowSelectionLinkedList run_filter(Table* table, ConditionFilter filter);
int should_show_col(char* col_names_split, char* col_name);