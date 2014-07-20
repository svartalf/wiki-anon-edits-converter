#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"

void output_write_header(FILE *f) {
	// TODO: write PostgreSQL data header for COPY BINARY input
}

void output_write_row(FILE *f, Revision *rev) {
	char *output;
	sprintf(output, "%d,%d,%d,%lu,%lu", rev->page_id, rev->old_id, rev->new_id, rev->ip, rev->timestamp);
	fwrite(output, strlen(output), 1, f);
}
