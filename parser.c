#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parser.h"

Revision *revision_create() {
	Revision *rev;
	rev = malloc(sizeof *rev);

	revision_clear(rev);

	return rev;
}

void revision_clear(Revision *rev) {
	rev->in_page = false;
	rev->in_revision = false;
	rev->id = 0;
	rev->parent_id = 0;
	rev->page_id = 0;
	rev->ip = 0;
	rev->timestamp = 0;

	rev->wait_for_id = false;
	rev->wait_for_parent_id = false;
	rev->wait_for_page_id = false;
	rev->wait_for_ip = false;
	rev->wait_for_timestamp = false;
};

bool revision_filled(Revision *rev) {
	return ((rev->ip != 0) && (rev->id != 0) && (rev->page_id != 0) && (rev->timestamp != 0));
}
