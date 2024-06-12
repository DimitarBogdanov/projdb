#include <stdio.h>
#include "data/dbdata.h"
#include "data/types.h"

int main() {
	init_vtypes();

	Database* db = create_db();
	Table* rat_tbl = add_table(db, "rat");
	add_column(rat_tbl, VT_INT, "id");
	add_column(rat_tbl, VT_INT, "age");

	int valueId = 5;
	int valueAge = 8;

	Row* row = add_row(rat_tbl);
	set_value(row, 0, &valueId);
	set_value(row, 1, &valueAge);
	printf("ID: %d\n", (int)get_value(row, 0));
	printf("Age: %d\n", (int)get_value(row, 1));

	free_entire_db(db);

	return 0;
}