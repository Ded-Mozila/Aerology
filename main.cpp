#include "settings.h"
#include "readingFile.h"
int main()
{

   	Settings app;
   	cout << "set" << '\n';			// директории файлов или папок
   	readingFile Code;
  	Code.OpenFile_(app.outDirectory, app.inDirectory);
  	//cout << app.outDirectory << '\n' << app.inDirectory << '\n';
	return 0;
}

