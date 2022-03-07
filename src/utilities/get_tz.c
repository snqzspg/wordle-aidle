#ifdef _WIN32
#include <timezoneapi.h>
#include <Windows.h>
#else
#define _GNU_SOURCE

#include <time.h>
#include "../terminal_helper/ccolours.h"
#endif // _WIN32

#include "get_tz.h"

long get_timezoneoffset_s() {
	#ifdef _WIN32
	TIME_ZONE_INFORMATION tz;
	DWORD dstmode = GetTimeZoneInformation(&tz);
	switch (dstmode) {
	case 1:
		return (tz.Bias + tz.StandardBias) * -60;
	case 2:
		return (tz.Bias + tz.DaylightBias) * -60;
	}
	return tz.Bias * -60;
	#elif defined(_GNU_SOURCE)
	// https://stackoverflow.com/questions/13804095/get-the-time-zone-gmt-offset-in-c
	time_t t = time(NULL);
	struct tm lt = {0};
	localtime_r(&t, &lt);
	return lt.tm_gmtoff;
	#else
	pgcg_set_warning_colour();
	printf("WARNING: This program has not found a way to get your timezone on your operating system.\n");
	printf("Today's word may be incorrect on this program.\n");
	pgcg_reset_colour();
	return 0;
	#endif // _WIN32
}
