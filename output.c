#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <netinet/in.h>

#include "parser.h"

void output_write_header(FILE *f) {
	const char *signature = "PGCOPY\n\377\r\n\0";
	const int32_t flags = 0;
	const int32_t header_extension = 0;

	fwrite(signature, 12, 1, f);
	fwrite(&flags, sizeof(int32_t), 1, f);
	fwrite(&header_extension, sizeof(int32_t), 1, f);
}

void write(FILE *f, int64_t value) {
	int32_t field_length = sizeof(int32_t);

	fwrite(&field_length, sizeof(int32_t), 1, f);
	fwrite(&value, sizeof(int64_t), 1, f);
}

void output_write_row(FILE *f, Revision *rev) {
	int16_t fields_num = 5;
	fwrite(&fields_num, sizeof(int16_t), 1, f);

	write(f, rev->id);
	write(f, rev->parent_id);
	write(f, rev->page_id);
	write(f, rev->ip);
	write(f, rev->timestamp);
}

void output_close(FILE *f) {
	const int16_t trailer = -1;
	fwrite(&trailer, sizeof(int16_t), 1, f);
	fclose(f);
}
