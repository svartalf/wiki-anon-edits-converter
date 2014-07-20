#ifndef _PARSER_H
#define _PARSER_H

#include <time.h>
#include <stdbool.h>

typedef struct Revision {
	bool in_page;
	bool in_revision;

	unsigned int page_id;
	unsigned int old_id;
	unsigned int new_id;
	unsigned long ip;
	time_t timestamp;

	bool wait_for_page_id;
	bool wait_for_old_id;
	bool wait_for_new_id;
	bool wait_for_ip;
	bool wait_for_timestamp;


} Revision;

Revision *revision_create();

void revision_clear(Revision *revision);

bool revision_filled(Revision *revision);

#endif
