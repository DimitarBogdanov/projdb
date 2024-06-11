#include <stdio.h>
#include "data/dbdata.h"

int main() {
	Database* db = create_db();
	Table* rat_tbl = add_table(db, "rat");
	ValueType* int_type = create_type(sizeof(int), "int");
	add_column(rat_tbl, int_type, "id");
	add_column(rat_tbl, int_type, "age");

	free_entire_db(db);

	return 0;
}