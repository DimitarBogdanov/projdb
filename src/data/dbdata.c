#include "dbdata.h"
#include <malloc.h>
#include <string.h>

ValueType* create_type(int width, const char* name, char* (*get_str_value_fn)(void*))
{
	ValueType* ptr = malloc(sizeof(ValueType));
	ptr->width = width;
	ptr->get_str_value_fn = get_str_value_fn;
	ptr->name = strdup(name);
	return ptr;
}

ColumnDef* add_column(Table* to_table, ValueType* value_type, const char* col_name)
{
	// TODO: Resize data on existing rows

	to_table->structure.num_cols++;
	if (to_table->structure.num_cols == 1)
	{
		// Ensure we malloc on the first row just in case
		to_table->structure.cols = calloc(1, sizeof(ColumnDef));
	}
	else
	{
		to_table->structure.cols = realloc(to_table->structure.cols, sizeof(ColumnDef) * to_table->structure.num_cols);
	}

	ColumnDef* ptr = &(to_table->structure.cols[to_table->structure.num_cols - 1]);
	ptr->type = value_type;
	ptr->name = strdup(col_name);

	return ptr;
}

Row* add_row(Table* to_table)
{
	Row* row = malloc(sizeof(Row));
	row->table_structure = &(to_table->structure);
	row->values = calloc(get_colwidth_sum(to_table), 1); // to ensure zeroing

	to_table->num_rows++;
	if (to_table->num_rows == 1)
	{
		// Ensure we malloc on the first row just in case
		to_table->rows = calloc(1, sizeof(Row*));
	}
	else
	{
		to_table->rows = realloc(to_table->rows, sizeof(Row*) * to_table->num_rows);
	}

	to_table->rows[to_table->num_rows - 1] = row;

	return row;
}

Table* add_table(Database* to_db, const char* name)
{
	Table* tbl = malloc(sizeof(Table));
	tbl->structure.name = strdup(name);

	to_db->num_tables++;
	if (to_db->num_tables == 1)
	{
		// Ensure we malloc on the first row just in case
		to_db->tables = malloc(sizeof(Table*));
	}
	else
	{
		to_db->tables = realloc(to_db->tables, sizeof(Table) * to_db->num_tables);
	}

	to_db->tables[to_db->num_tables - 1] = tbl;

	return tbl;
}

Database* create_db()
{
	Database* db = malloc(sizeof(Database));
	return db;
}

void* get_value(Row* row, int col_idx)
{
	if (col_idx >= row->table_structure->num_cols)
	{
		return NULL;
	}

	size_t offset = 0;

	for (int i = 0; i < col_idx; i++)
	{
		offset += row->table_structure->cols[i].type->width;
	}

	return (row->values + offset);
}

void set_value(Row* row, int col_idx, void* value)
{
	if (col_idx >= row->table_structure->num_cols)
	{
		return;
	}

	size_t offset = 0;

	for (int i = 0; i < col_idx; i++)
	{
		offset += row->table_structure->cols[i].type->width;
	}

	char* values_ptr = row->values;
	char* offset_ptr = values_ptr + offset;

	int width = row->table_structure->cols[col_idx].type->width;

	memcpy(offset_ptr, value, width);
}

void free_table_with_rows(Table* tbl)
{
	for (int i = 0; i < tbl->num_rows; ++i)
	{
		free(tbl->rows[i]->values);
	}
	free(tbl);
}

void free_entire_db(Database* db)
{
	for (int i = 0; i < db->num_tables; ++i)
	{
		free_table_with_rows(db->tables[i]);
	}

	free(db);
}

size_t get_colwidth_sum(Table* tbl)
{
	size_t width = 0;

	for (int i = 0; i < tbl->structure.num_cols; ++i)
	{
		width += tbl->structure.cols[i].type->width;
	}

	return width;
}

Table* get_table(Database* db, char* table_name)
{
	for (int i = 0; i < db->num_tables; ++i)
	{
		Table* t = db->tables[i];
		if (!strcmp(t->structure.name, table_name))
		{
			return t;
		}
	}

	return NULL;
}