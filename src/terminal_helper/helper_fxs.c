#include <stdio.h>
#include <stdlib.h>

#include "../utilities/input-helper.h"
#include "helper_fxs.h"

void clear_console() {
	#ifdef _WIN32
	if (system("cls") == -1) printf("\n\n------------------------------\n\n");
	#elif defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
	if (system("clear") == -1) printf("\n\n------------------------------\n\n");
	#else
	printf("\n\n------------------------------\n\n");
	#endif // _WIN32
}

void pause_console() {
	#ifdef _WIN32
	system("pause");
	#else
	printf("Press <enter> to continue");
	char* d = ask_user();
	free(d);
	#endif // _WIN32
}
