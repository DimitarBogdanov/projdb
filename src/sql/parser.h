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

typedef enum {
	PARSEOP_SELECT
} ParseOperationIdx;

typedef struct {
	ParseOperationIdx op_type; // should always be PARSEOP_SELECT
	ConditionFilter filter;
	char* db_name;
	char* table_name;
} ParseOperationSelect;

typedef union {
	ParseOperationIdx tag;

	ParseOperationSelect select;
} ParseOperation;

typedef struct {
	int has_errors;
	int op_count;
	ParseOperation* operations_arr;
} ParseResult;

ParseResult parse(TokenLinkedList* tokens, Database* db);