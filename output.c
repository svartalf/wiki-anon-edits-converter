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

	fwrite(signature, strlen(signature) + 1, 1, f); // We should write \0 too
	fwrite(&flags, sizeof(int32_t), 1, f);
	fwrite(&header_extension, sizeof(int32_t), 1, f);
}

void output_write_row(FILE *f, Revision *rev) {
	int16_t fields_num = 5;
	fwrite(&fields_num, sizeof(int16_t), 1, f);

	int64_t field_length = sizeof(int64_t);

	fwrite(&field_length, field_length, 1, f);
	fwrite(&rev->id, sizeof(int64_t), 1, f);

	fwrite(&field_length, field_length, 1, f);
	fwrite(&rev->parent_id, sizeof(int64_t), 1, f);

	fwrite(&field_length, field_length, 1, f);
	fwrite(&rev->page_id, sizeof(int64_t), 1, f);

	fwrite(&field_length, field_length, 1, f);
	fwrite(&rev->ip, sizeof(int64_t), 1, f);

	fwrite(&field_length, field_length, 1, f);
	fwrite(&rev->timestamp, sizeof(int64_t), 1, f);
}

void output_close(FILE *f) {
	const int16_t trailer = -1;
	fwrite(&trailer, sizeof(int16_t), 1, f);
	fclose(f);
}
