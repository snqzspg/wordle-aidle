#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_time_format(double secs) {
	if (secs >= 86400) {
		printf("%dd ", (int)secs / 86400);
	}
	if (secs >= 3600) {
		printf("%dh ", ((int)secs / 3600) % 24);
	}
	if (secs >= 60) {
		printf("%dm ", ((int)secs / 60) % 60);
	}
	printf("%fs", fmod(secs, 60));
}

void log_time_format(FILE* f, double secs) {
	if (secs >= 86400) {
		fprintf(f, "%dd ", (int)secs / 86400);
	}
	if (secs >= 3600) {
		fprintf(f, "%dh ", ((int)secs / 3600) % 24);
	}
	if (secs >= 60) {
		fprintf(f, "%dm ", ((int)secs / 60) % 60);
	}
	fprintf(f, "%fs", fmod(secs, 60));
}

void print_time_format_2dp(double secs) {
	if (secs >= 86400) {
		printf("%d:", (int)secs / 86400);
	}
	if (secs >= 3600) {
		printf(secs >= 86400 ? "%02d:" : "%d:", ((int)secs / 3600) % 24);
	}
	if (secs >= 60) {
		printf(secs >= 3600 ? "%02d:" : "%d:", ((int)secs / 60) % 60);
		double s = fmod(secs, 60);
		printf(s < 10 ? "0%.2f" : "%.2f", s);
	} else {
		printf("%.2fs", secs);
	}
}
