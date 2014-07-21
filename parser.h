#ifndef _PARSER_H
#define _PARSER_H

#include <time.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct Revision {
	bool in_page;
	bool in_revision;

	uint64_t id;
	uint64_t parent_id;
	uint64_t page_id;
	uint64_t ip;
	time_t timestamp;

	bool wait_for_id;
	bool wait_for_parent_id;
	bool wait_for_page_id;
	bool wait_for_ip;
	bool wait_for_timestamp;


} Revision;

Revision *revision_create();

void revision_clear(Revision *revision);

bool revision_filled(Revision *revision);

#endif
