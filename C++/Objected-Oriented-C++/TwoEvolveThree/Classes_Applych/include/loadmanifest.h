#ifndef __MANIFEST_H__
#define __MANIFEST_H__
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <malloc.h>

using namespace std;

void loadfest(map<string,string>& pro, vector<string> xml);
static void readxml(map<string, string>& pro, int fd);


#endif
