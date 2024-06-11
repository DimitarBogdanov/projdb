#include "dbdata.h"
#include <malloc.h>
#include <string.h>

ValueType* create_type(int width, const char* name)
{
	ValueType* ptr = malloc(sizeof(ValueType));
	ptr->width = width;
	ptr->name = strdup(name);
	return ptr;
}

ColumnDef* add_column(Table* to_table, ValueType* value_type, const char* col_name)
{
	// TODO: Resize data on existing rows
	ColumnDef* ptr = malloc(sizeof(ColumnDef));
	ptr->type = value_type;
	ptr->name = strdup(col_name);
	return ptr;
}

Row* add_row(Table* to_table, void* values)
{
	Row* row = malloc(sizeof(Row));
	row->table_structure = &(to_table->structure);
	row->values = values;

	to_table->num_rows++;
	if (to_table->num_rows == 1)
	{
		// Ensure we malloc on the first row just in case
		to_table->rows = malloc(sizeof(Row*));
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

	return &(row->values[offset]);
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