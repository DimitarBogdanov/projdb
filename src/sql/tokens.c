#include "tokens.h"
#include <stddef.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>

#define BUFF_SIZE_TOK 32

int is_only_digits_in_buff(char* buff, int* tok_len)
{
	if (*tok_len == 0)
	{
		return 0;
	}

	for (int i = 0; i < *tok_len; ++i)
	{
		if (!isdigit(buff[i]))
		{
			return 0;
		}
	}

	return 1;
}

int is_buff_eq(char* buff, int* tok_len, const char* value)
{
	if (strlen(value) != *tok_len)
	{
		return 0;
	}

	for (int i = 0; i < *tok_len; ++i)
	{
		if (tolower(buff[i]) != tolower(value[i]))
		{
			return 0;
		}
	}

	return 1;
}

void add_ch(char* buff, int* tok_len, char ch)
{
	if (*tok_len + 1 >= BUFF_SIZE_TOK)
	{
		perror("token too long");
		return;
	}

	buff[*tok_len] = ch;
	(*tok_len)++;
}

void append_tok(TokenLinkedList* list, Token* tok)
{
	if (list->num_tokens == 0)
	{
		list->start = tok;
		tok->prev = NULL;
	}
	else
	{
		list->end->next = tok;
		tok->prev = list->end;
	}
	
	tok->next = NULL;
	list->end = tok;

	list->num_tokens++;
}

void push_tok_direct(TokenLinkedList* list, TokenType tok_type)
{
	Token* tok = malloc(sizeof(Token));
	
	tok->has_value = 0;
	tok->value = NULL;
	tok->type = tok_type;

	append_tok(list, tok);
}

void push_tok_value(TokenLinkedList* list, TokenType tok_type, char* buff, int* tok_len)
{
	Token* tok = malloc(sizeof(Token));

	tok->has_value = *tok_len;
	tok->value = calloc(sizeof(char), *tok_len + 1);
	strncpy(tok->value, buff, *tok_len);

	tok->type = tok_type;

	append_tok(list, tok);
}

void push_tok(TokenLinkedList* list, char* buff, int* tok_len)
{
	if (*tok_len == 0)
	{
		return;
	}

	if (is_buff_eq(buff, tok_len, "SELECT"))
	{
		push_tok_direct(list, TOK_SELECT);
	}
	else if (is_buff_eq(buff, tok_len, "FROM"))
	{
		push_tok_direct(list, TOK_FROM);
	}
	else
	{
		push_tok_value(list, TOK_ID, buff, tok_len);
	}
	
	*tok_len = 0;
}

TokenLinkedList tokenize(size_t len_src, char* src_str)
{
	TokenLinkedList list;

	char buff[BUFF_SIZE_TOK];
	int tok_len = 0;

	size_t head = 0;
	while (head < len_src)
	{
		char curr = src_str[head++];
		if (!curr)
		{
			break;
		}

		if (isspace(curr))
		{
			push_tok(&list, buff, &tok_len);
		}
		else if (isdigit(curr) || isalpha(curr))
		{
			add_ch(buff, &tok_len, curr);
		}
		else if (curr == '*')
		{
			push_tok(&list, buff, &tok_len);
			tok_len = 0;
			push_tok_direct(&list, TOK_ASTERISK);
		}
		else if (curr == ',')
		{
			push_tok(&list, buff, &tok_len);
			tok_len = 0;
			push_tok_direct(&list, TOK_COMMA);
		}
	}

	return list;
}