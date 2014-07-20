#ifndef _OUTPUT_H
#define _OUTPUT_H

#include "parser.h"

void output_write_header(FILE *f);

void output_write_row(FILE *f, Revision *rev);

#endif

