#include <stdio.h>

#include "../utilities/file_util.h"

#include "usage_info.h"

void print_usage_info(const char* arg0) {
	const char* fn = get_path_leaf(arg0);
	printf("usage: %s [--no_clear_console]\n", fn);
	printf("       no_clear_console  Optional, this option prevents Wordle Aidle\n");
	printf("                         from clearing your terminal entries.\n");
}
