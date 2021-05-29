#ifndef __MYTBF_H__
#define __MYTBF_H__

#define MYTBF_MAX 1024
typedef void mytbf_t;

mytbf_t* mytbf_init(int cps, int burst);

int mytbf_fetchtoken(mytbf_t* tbf, int size);

int mytbf_returntoken(mytbf_t* tbf, int size);

int mytbf_destroy(mytbf_t* ptr);

#endif
