#pragma once

#include "data/dbdata.h"

/*
 * repl.h
 * Contains methods for running the console REPL loop.
 */

// 0 = cancel REPL
int run_repl(Database* db);