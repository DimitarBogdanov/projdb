#include "dbdata.h"
#include "types.h"
#include <stdint.h>
#include <malloc.h>
#include <string.h>

char* _strval_int(void* value)
{
	int32_t* ptr = (int32_t*)value;

	int length = snprintf( NULL, 0, "%d", *ptr);
	char* str = malloc( length + 1 );
	snprintf( str, length + 1, "%d", *ptr );

	return str;
}

char* _strval_bool(void* value)
{
	int* ptr = (int*)value;

	if (*ptr)
	{
		char* str = malloc(sizeof(char) * 5);
		strcpy(str, "TRUE");
		return str;
	}
	else
	{
		char* str = malloc(sizeof(char) * 6);
		strcpy(str, "FALSE");
		return str;
	}
}

char* _strval_text(void* value)
{
	char* ptr = (char*)value;
	char* copy = strdup(ptr);
	return copy;
}

void init_vtypes() {
	VT_INT = create_type(4, "INT", *_strval_int);
	VT_BOOL = create_type(1, "BOOL", *_strval_bool);
	VT_TEXT_32 = create_type(32, "TEXT_32", *_strval_text);
}