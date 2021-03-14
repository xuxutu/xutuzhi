#include "csvloader.h"


int main(void)
{
	Csvloader csv("inputfile/machine_readable.csv");

	auto head = csv.getHead();

	return 0;
}
