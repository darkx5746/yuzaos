/*
 * asctime.c
 * Original Author:	G. Haley
 *
 * Converts the broken down time in the structure pointed to by tim_p into a
 * string of the form
 *
 * Wed Jun 15 11:38:07 1988\n\0
 *
 * Returns a pointer to the string.
 */

/*
FUNCTION
<<asctime>>---format time as string

INDEX
	asctime
INDEX
	_asctime_r

ANSI_SYNOPSIS
	#include <time.h>
	char *asctime(const struct tm *<[clock]>);
	char *_asctime_r(const struct tm *<[clock]>, char *<[buf]>);

TRAD_SYNOPSIS
	#include <time.h>
	char *asctime(<[clock]>)
	struct tm *<[clock]>;
	char *asctime_r(<[clock]>)
	struct tm *<[clock]>;
	char *<[buf]>;

DESCRIPTION
Format the time value at <[clock]> into a string of the form
. Wed Jun 15 11:38:07 1988\n\0
The string is generated in a static buffer; each call to <<asctime>>
overwrites the string generated by previous calls.

RETURNS
A pointer to the string containing a formatted timestamp.

PORTABILITY
ANSI C requires <<asctime>>.

<<asctime>> requires no supporting OS subroutines.
*/

/* Includes 
 * - Library */
#include <orangeos.h>
#include "tls.h"
#include <stringdef.h>
//#include <stdio.h>
#include <time.h>
#include <memory.h>


/* Reentrency version of asctime 
 * Modified implementation by newlib */
char *asctime_r(const struct tm *__restrict tim_p, char *__restrict result)
{
	static const char day_name[][4] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static const char mon_name[][4] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};

	snprintf(result, 256, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",day_name[tim_p->tm_wday], mon_name[tim_p->tm_mon], tim_p->tm_mday, tim_p->tm_hour, tim_p->tm_min, tim_p->tm_sec, 1900 + tim_p->tm_year);
	return result;
}

/* Formats a given timebuffer to a 
 * string of format Www Mmm dd hh:mm:ss yyyy */
char *asctime(const struct tm *tim_p)
{
	char *ascbuf = &tls_current()->asc_buffer[0];
	memset(ascbuf, 0, sizeof(tls_current()->asc_buffer));
	return asctime_r(tim_p, ascbuf);
}
