#pragma once

#include "parser.h"
#include "../data/dbdata.h"

/*
 * condfilters.h
 * Contains filters for selection queries.
 */

ConditionFilter condfilter_all();

RowSelectionLinkedList run_filter(Table* table, ConditionFilter filter);