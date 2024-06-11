#include <stdio.h>
#include "data/dbdata.h"
#include "data/types.h"

int main() {
	init_vtypes();

	Database* db = create_db();
	Table* rat_tbl = add_table(db, "rat");
	add_column(rat_tbl, VT_INT, "id");
	add_column(rat_tbl, VT_INT, "age");

	free_entire_db(db);

	return 0;
}