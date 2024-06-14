#include "repl.h"
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include "sql/tokens.h"
#include "sql/parser.h"
#include "sql/condfilters.h"
#include <malloc.h>
#include <string.h>

void print_padding(char ch, int len)
{
    for (int i = 0; i < len; ++i)
    {
        putc(ch, stdout);
    }
}

void print_sep_line(int width_per_col, int num_cols)
{
    putc('+', stdout);
    for (int i = 0; i < num_cols; ++i)
    {
        print_padding('-', width_per_col + 2);
        putc('+', stdout);
    }
    puts("");
}

void print_full_width(char* str, int max_width)
{
    int len = strlen(str);
    if (len <= max_width)
    {
        printf("%s", str);
        int diff = max_width - len;
        if (diff != 0)
        {
            print_padding(' ', diff);
        }
    }
    else
    {
        // 1. inject ... into string and a null byte afterwards
        // 2. print
        // 3. restore the modified string
        // 4. ???
        // 5. profit
        int offsets[] = { max_width - 4, max_width - 3, max_width - 2, max_width - 1 };
        char temp[4];
        for (int i = 0; i < 4; ++i)
        {
            temp[i] = str[offsets[i]];
        }
        for (int i = 0; i < 3; ++i)
        {
            str[offsets[i]] = '.';
        }
        str[offsets[3]] = '\0';
        printf("%s", str);

        for (int i = 0; i < 4; ++i)
        {
            str[offsets[i]] = temp[i];
        }
    }
}

void print_rows(TableDef* table_structure, RowSelectionLinkedList rows)
{
    const int max_width = 100; // completely arbitrary
    int width_per_col = max_width / table_structure->num_cols;

    // Header
    print_sep_line(width_per_col, table_structure->num_cols);
    printf("| ");
    for (int i = 0; i < table_structure->num_cols; ++i)
    {
        char* col_name = (char*) table_structure->cols[i].name;
        print_full_width(col_name, width_per_col);
        printf(" | ");
    }
    puts("");
    print_sep_line(width_per_col, table_structure->num_cols);

    // Data
    printf("| ");
    SelectedRow* row = rows.start;
    while (row)
    {
        Row* ref = row->ref;

        for (int i = 0; i < table_structure->num_cols; ++i)
        {
            ColumnDef col = table_structure->cols[i];
            void* value_ptr = get_value(ref, i);

            char* value = col.type->get_str_value_fn(value_ptr);
            print_full_width(value, width_per_col);
            printf(" | ");
        }

        row = row->next;
    }
    puts("");
    print_sep_line(width_per_col, table_structure->num_cols);
}

void run_op(Database* db, ParseOperation* op)
{
    if (op->tag == PARSEOP_SELECT)
    {
        // select operation
        ParseOperationSelect selectOp = op->select;
        Table* t = get_table(db, selectOp.table_name);
        if (!t)
        {
            fprintf(stderr, "unknown table\n");
            return;
        }

        RowSelectionLinkedList result = run_filter(t, selectOp.filter);

        print_rows(&t->structure, result);
        printf("\nselected %d rows\n", result.num_rows);

        // Free results
        SelectedRow* curr = result.start;
        while (curr)
        {
            SelectedRow* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
}

void execute_result(Database* db, ParseResult res)
{
    if (res.has_errors)
    {
        return;
    }

    // TODO: Support multiple operations
    if (!res.operations_arr)
    {
        return;
    }

    run_op(db, res.operations_arr);
}

void run_repl(Database* db)
{
    char* line = NULL;
    size_t len = 0;
    ssize_t read_length = 0;
    printf("> ");
    read_length = getline(&line, &len, stdin);
    if (read_length == -1)
        return;

    TokenLinkedList* tok_result = tokenize(read_length, line);
    ParseResult parse_result = parse(tok_result, db);

    execute_result(db, parse_result);
    puts("");

    free(tok_result); // todo: why is this a pointer? the structure is tiny
    free(line);
}