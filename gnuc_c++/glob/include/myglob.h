#ifndef __MYGLOB_H__
#define __MYGLOB_H__
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

#ifndef gl_size_t
#define gl_size_t	size_t
#endif

#ifndef gl_stat_t
#define gl_stat_t	struct stat
#endif


typedef struct
{
	gl_size_t gl_pathc;		/* Count of total paths so far. */
	gl_size_t gl_matchc;	/* Count of paths matching pattern. */
	gl_size_t gl_offs;		/* Reserved at beginning of gl_pathv. */
	int gl_flags;			/* Copy of flags parameter to glob. */
	char **gl_pathv;			/* List of paths matching pattern. */	
} myglob_t;

#define MAXPATHLEN 1024
#define	GLOB_ABORTED	(-2)	/* Unignored error. */



#define	GLOB_ABEND	GLOB_ABORTED
#define	GLOB_APPEND		0x0001	/* Append to output from previous call. */
#define	GLOB_NOCHECK	0x0010	/* Return pattern itself if nothing matches. */
#define	GLOB_MAGCHAR	0x0100	/* Pattern had globbing characters. */

int	myglob(const char *pattern, int flags,  int (*errfunc)(const char *, int), myglob_t *pglob);

void myglobfree(myglob_t *);


#endif
