#pragma once

#include <stddef.h>

/*
 * tokens.h
 * Contains tSQL tokenizer structures and functions.
 * (tSQL = tiny SQL)
 */

typedef enum {
	TOK_SELECT,
	TOK_FROM,
	TOK_WHERE,
	TOK_ID,

	TOK_ASTERISK,
	TOK_COMMA
} TokenType;

typedef struct _tok {
	TokenType type;
	int has_value;
	char* value;
	struct _tok* prev;
	struct _tok* next;
} Token;

typedef struct {
	int num_tokens;
	Token* start;
	Token* end;
} TokenLinkedList;

TokenLinkedList tokenize(size_t len_src, char* src_str);