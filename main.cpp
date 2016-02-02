#include "readingFile.h"
int main()
{
	readingFile Code;
	local_time start(2015,5,1,0);
	local_time end(2015,12,31,12);
	Code.OpenDirPeriod(start,end);
	return 0;
}
