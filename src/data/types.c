#include "dbdata.h"
#include "types.h"

void init_vtypes() {
	VT_INT = create_type(32, "INT");
	VT_BOOL = create_type(1, "BOOL");
	VT_TEXT_30 = create_type(30, "TEXT_30");
}