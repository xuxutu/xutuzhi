#ifndef SETENV__
#define SETENV__
#include <stdio.h>
#include <stdlib.h>
/*设置环境变量,如果存在并且rewrite的值为真时，覆盖原来的环境变量
 *
 */


extern int setenv(const char *name, const char *value, int rewrite);


#endif
