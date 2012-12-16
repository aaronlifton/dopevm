#ifndef _UTIL_H
#define _UTIL_H

void panic(char *, ...);

#ifdef __GNUC__
	__attribute__ ((format (printf, 1, 2)))
#endif
;

// http://unixwiz.net/techtips/gnu-c-attributes.html

#define TRUE 1
#define FALSE 0

#endif /* defined(_UTIL_H) */
