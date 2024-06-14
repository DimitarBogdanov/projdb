#pragma once

#include "tokens.h"
#include "../data/dbdata.h"

/*
 * parser.h
 * Parses the tokens into sensible commands.
 */

typedef struct _srow {
	Row* ref;
	struct _srow* next;
} SelectedRow;

typedef struct {
	int num_rows;
	SelectedRow* start;
} RowSelectionLinkedList;

typedef struct {
	int (*filter_func)(Row*); // returns != 0 if the row should be selected
} ConditionFilter;

typedef struct {
	ConditionFilter filter;
	char* db_name;
	char* table_name;
} SelectionOperation;

void parse(TokenLinkedList* tokens, Database* db);