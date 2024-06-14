#include "parser.h"
#include "tokens.h"
#include "../data/dbdata.h"
#include <string.h>
#include <stdio.h>
#include "condfilters.h"
#include "malloc.h"

typedef struct {
	Token* curr;
	int did_parsing_fail;
	ParseOperation* op;
} ParseHelper;

void flag_parsing_as_failed(ParseHelper* parser)
{
	parser->did_parsing_fail = 1;
}

void add_parse_op(ParseHelper* parser, ParseOperation* op)
{
	parser->op = op;
}

void next_token(ParseHelper* parser)
{
	if (!parser->curr)
	{
		return;
	}

	parser->curr = parser->curr->next;
}

Token* peek_tok(ParseHelper* parser)
{
	if (!parser->curr)
	{
		return NULL;
	}

	return parser->curr->next;
}

int has_tok(ParseHelper* parser)
{
	return parser->curr != NULL;
}

int tok_is(ParseHelper* parser, TokenType tok_type)
{
	return parser->curr && parser->curr->type == tok_type;
}

char* expect_name(ParseHelper* parser)
{
	if (!has_tok(parser))
	{
		fprintf(stderr, "no token to expect name on");
		flag_parsing_as_failed(parser);
		return NULL;
	}
	
	if (!tok_is(parser, TOK_ID))
	{
		fprintf(stderr, "expected name");
		flag_parsing_as_failed(parser);
		return NULL;
	}

	char* value = strdup(parser->curr->value);
	return value;
}

int is_select(ParseHelper* parser)
{
	return tok_is(parser, TOK_SELECT);
}

void parse_select(ParseHelper* parser)
{
	next_token(parser);

	ConditionFilter filter;
	if (tok_is(parser, TOK_ASTERISK))
	{
		next_token(parser);
		filter = condfilter_all();
	}
	else
	{
		// TODO: Support specific column select
		fprintf(stderr, "expected *");
		flag_parsing_as_failed(parser);
		return;
	}
	
	if (tok_is(parser, TOK_FROM))
	{
		next_token(parser);
	}
	else
	{
		fprintf(stderr, "expected FROM");
		flag_parsing_as_failed(parser);
		return;
	}

	char* db_name = NULL;
	char* table_name = expect_name(parser);

	ParseOperationSelect* op = malloc(sizeof(ParseOperationSelect));
	op->op_type == PARSEOP_SELECT;
	op->db_name = NULL;
	op->table_name = table_name;
	op->filter = condfilter_all();
	add_parse_op(parser, (ParseOperation*) op);
}

ParseResult parse(TokenLinkedList* tokens, Database* db)
{
	ParseHelper helper;
	helper.did_parsing_fail = 0;
	helper.op = NULL;
	helper.curr = tokens->start;

	while (helper.curr)
	{
		if (is_select(&helper))
		{
			parse_select(&helper);
		}
		else
		{
			// ???
			flag_parsing_as_failed(&helper);
			break;
		}

		break; // TODO: support more than one command
	}

	ParseResult result;
	result.has_errors = helper.did_parsing_fail;
	result.op_count = 1;
	result.operations_arr = helper.op;
	return result;
}