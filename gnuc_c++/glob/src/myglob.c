#include "myglob.h"
#include "assert.h"

#define M_ASCII		  0x00ff
#define M_PROTECT     (char)0x40
#define M_QUOTE		  (char)0x80
#define CHAR(c)       ((Char)((c)&M_ASCII))
#define M_RNG         META('-')
#define M_END         META(']')

#define EOS           '\0'
#define QUOTE         '\\'
#define TILDE         '~'
#define LBRACKET      '['
#define RBRACKET      ']'
#define NOT           '!'
#define RANGE         '-'


#define META(c)       ((char)((c)|M_QUOTE))		//0x80  1000 0000 
#define M_SET         META('[')

//针对‘~’字符处理
static const char *myglobtilde(const char *pattern, char *patbuf, size_t patsize, myglob_t *pglob)
{
	const char* h;
	const char* p;
	char* b;
	char* d;
	char* pend = &patbuf[patsize / sizeof(char)];

	pend --;

	assert(pattern != NULL);
	assert(patbuf != NULL);
	assert(pglob != NULL);

	if (*pattern != TILDE || !(pglob->gl_flags & GLOB_TILDE))
    return pattern;
}

static int myglob0(const char* pattern, myglob_t *pglob, size_t *limit)
{
	const char* qpatnext;	
	char patbuf[MAXPATHLEN + 1];
	gl_size_t oldpathc;
	char *bufnext;

	assert(pattern != NULL);
	assert(pglob != NULL);


	if((qpatnext = myglobtilde(pattern, patbuf, sizeof(patbuf), pglob)) == NULL)	
		return GLOB_ABEND;

	oldpathc = pglob->gl_pathc;
	bufnext = patbuf;
	

	/* We don't need to check for buffer overflow any more. */
	while((c = *qpatnext++) != EOF)
	{
		switch(c)
		{
			case LBRACKET:
			{
				c = *qpatnext;
				if(c == NOT)
					++qpatnext;//[]里找到‘!’，就跳过

				//如果遇到'\0或']',将'['存入patbuf
				if(*qpatnext == EOS || g_strchr(qpatnext+1, RBRACKET) == NULL)
				{
					*bufnext++ = LBRACKET;
					if (c == NOT)			//[!]
						--qpatnext;

					break;
				}

				*bufnext++ = M_SET;

			    if (c == NOT)
					*bufnext++ = M_NOT;

				c = *qpatnext++;

				do
				{
					*bufnext++ = CHAR(c);
					if (*qpatnext == RANGE && (c = qpatnext[1]) != RBRACKET)
					{
						*bufnext++ = M_RNG;
						*bufnext++ = CHAR(c);
						qpatnext += 2;
					}
				}while((c = *qpatnext++) != RBRACKET)

				pglob->gl_flags |= GLOB_MAGCHAR;
				*bufnext++ = M_END;

				break;	
			}// case LBRACKET end


			
		}//switch end

	

	}//while end
}

int myglob(const char *pattern, int flags,  int (*errfunc)(const char *, int), myglob_t *pglob)
{
	const unsigned char *patnext; 
	int c;
	char *bufnext, *bufend;
	char patbuf[MAXPATHLEN + 1];


	/* 0 = malloc(), 1 = stat(), 2 = readdir() */
	size_t limit[] = {0, 0, 0};
	
	assert(pattern);

	patnext = (const unsigned char *)pattern;

	
	if(!(flags & GLOB_APPEND))
	{
		pglob->gl_pathc = 0;
		pglob->gl_pathv = 0;	
	}

	bufnext = patbuf;
	bufend = bufnext + MAXPATHLEN;

    /* Protect the quoted characters. */
	while (bufnext < bufend && (c = *patnext++) != EOS)
	{
		if(c == QUOTE)
		{
			if ((c = *patnext++) == EOS)
			{
				c = QUOTE;
				--patnext;
			}

			*bufnext++ = c | M_PROTECT;		//0x40 | 0100 0000
											//		 0011 1010		
		}									//		 0111 1010				
		else								//		 0010 1011 01101000	
		{
			*bufnext++ = c;
		}
	}

	*bufnext = EOS;
	puts(patbuf);

	return myglob0(patbuf, pglob, limit);
}
