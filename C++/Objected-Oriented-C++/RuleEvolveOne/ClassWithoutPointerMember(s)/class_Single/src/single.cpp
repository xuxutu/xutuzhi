#include "single.h"

Single& Single::getInstance()
{
	static Single s;
	return s;

}
