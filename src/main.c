#include <stdio.h>
#include "data/dbdata.h"
#include "data/types.h"
#include "repl.h"

Database* generate_test_db();

int main() {
	init_vtypes();

	Database* db = generate_test_db();

	while (1) {
		run_repl(db);
	}

	free_entire_db(db);

	return 0;
}

Database* generate_test_db()
{
	Database* db = create_db();
	Table* rat_tbl = add_table(db, "rat");
	add_column(rat_tbl, VT_INT, "id");
	add_column(rat_tbl, VT_INT, "age");

	int valueId = 5;
	int valueAge = 8;

	Row* row = add_row(rat_tbl);
	set_value(row, 0, &valueId);
	set_value(row, 1, &valueAge);

	return db;
}