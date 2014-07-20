#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <time.h>

#include "libxml/xmlversion.h"
#include "libxml/xmlstring.h"
#include "libxml/xmlreader.h"

#include "parser.h"
#include "output.h"

#ifndef __USE_XOPEN
	#define __USE_XOPEN
#endif

#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif

static void processNode(xmlTextReaderPtr reader, Revision *rev) {
	if (xmlTextReaderNodeType(reader) == 15) { // EndElement
		return;
	}
    const xmlChar *name, *value;

    const xmlChar *node_page = (xmlChar *) "page";
    const xmlChar *node_revision = (xmlChar *) "revision";
    const xmlChar *node_id = (xmlChar *) "id";
    const xmlChar *node_ip = (xmlChar *) "ip";
    const xmlChar *node_parent_id = (xmlChar *) "parent_id";
    const xmlChar *node_timestamp = (xmlChar *) "timestamp";

    name = xmlTextReaderConstName(reader);
    if (name == NULL)
    	return;

    value = xmlTextReaderConstValue(reader);

    if (xmlTextReaderNodeType(reader) == 3) { // Text node
    	if (rev->wait_for_page_id == true) {
			rev->page_id = atoi((char *) value);
    		rev->wait_for_page_id = false;
    		return;
    	}

    	if (rev->wait_for_new_id == true) {
			rev->new_id = atoi((char *) value);
    		rev->wait_for_new_id = false;
    		return;
    	}

    	if (rev->wait_for_old_id == true) {
        	rev->old_id = atoi((char *) value);
        	rev->wait_for_old_id = false;
        	return;
    	}

    	if (rev->wait_for_ip == true) {
    		// TODO: check if PostgreSQL is fine with network byte order
        	rev->ip = inet_addr(value);
        	rev->wait_for_ip = false;
        	return;
    	}

    	if (rev->wait_for_timestamp == true) {
    		struct tm ts;
    		memset(&ts, 0, sizeof(struct tm));
			// TODO: check for a NULL return
    		strptime(value, "%Y-%m-%dT%H:%M:%SZ", &ts);
    		rev->timestamp = mktime(&ts);
    		rev->wait_for_timestamp = false;
    		return;
    	}
    }

    if ((rev->in_page == false) && (xmlStrEqual(name, node_page) == 1)) {
    	rev->in_page = true;
    	rev->in_revision = false;
    }

    if ((rev->in_page == true) && (xmlStrEqual(name, node_revision) == 1)) {
    	rev->in_page = false;
    	rev->in_revision = true;
    }

    if (xmlStrEqual(name, node_id) == 1) {
		if (rev->in_page == true) {
			rev->wait_for_page_id = true;
			return;
		}

		if (rev->in_revision == true) {
			rev->wait_for_new_id = true;
			return;
		}
    }

    if ((rev->in_revision == true) && (xmlStrEqual(name, node_parent_id) == 1)) {
    	rev->wait_for_old_id = true;
    	return;
    }
    if ((rev->in_revision == true) && (xmlStrEqual(name, node_ip) == 1)) {
    	rev->wait_for_ip = true;
    	return;
    }

    if ((rev->in_revision == true) && (xmlStrEqual(name, node_timestamp) == 1)) {
    	rev->wait_for_timestamp = true;
    	return;
    }

}

int main(int argc, char *argv[]) {
	LIBXML_TEST_VERSION

	// xmlTextReaderPtr reader = xmlReaderForFile("/run/media/svartalf/storage/wikipedia/ruwiki-20140706-pages-meta-history1.xml", NULL, 0);
	xmlTextReaderPtr reader = xmlReaderForFile("/tmp/test.xml", NULL, 0);

	FILE *output = fopen("/tmp/output.csv", "w");
	Revision *rev = revision_create();

	int result = xmlTextReaderRead(reader);
	while (result == 1) {
		processNode(reader, rev);
		result = xmlTextReaderRead(reader);

		if (revision_filled(rev)) {
			output_write_row(output, rev);
			revision_clear(rev);
		}

	}
	fclose(output);
	xmlFreeTextReader(reader);
	if (result != 0) {
		fprintf(stderr, "failed to parse: %d\n", result);
	}
	xmlCleanupParser();
	xmlMemoryDump();

	return 0;
}
