#pragma once

#include <stddef.h>

/*
 * dbdata.h
 * Contains structs and functions for basic data handling and table structure.
 */

//
// Structures
//

typedef struct {
	size_t width;
	const char* name;
} ValueType;

typedef struct {
	ValueType* type;
	const char* name;
} ColumnDef;

typedef struct {
	const char* name;
	int num_cols;
	ColumnDef* cols;
} TableDef;

typedef struct {
	TableDef* table_structure;
	char* values;
} Row;

typedef struct {
	TableDef structure;
	int num_rows;
	Row** rows;
} Table;

typedef struct {
	int num_tables;
	Table** tables;
} Database;


//
// Functions
//

ValueType* create_type(int width, const char* name);
ColumnDef* add_column(Table* to_table, ValueType* value_type, const char* col_name);
Row* add_row(Table* to_table);
Table* add_table(Database* to_db, const char* name);
Database* create_db();

void free_row(Row* row);
void free_table_with_rows(Table* tbl);
void free_entire_db(Database* db);

size_t get_colwidth_sum(Table* tbl);

void* get_value(Row* row, int col_idx);
void set_value(Row* row, int col_idx, void* value);