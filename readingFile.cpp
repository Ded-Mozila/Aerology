#include "readingFile.h"

readingFile::readingFile(void)
{
}
void readingFile::OpenFile_(string name, string file_next)
{
	TTXX.data = 0 ; 
	file.open( name.c_str(), ios_base::in);
	if(!file)
	{
		perror("Error open file!");
		cin.get();
		exit(1);
	}
	else
	{
		readF();
		file.close();
		cout << "readGood";
		TTXX.WriteFile(file_next);
		cout << "writeGood";
	}
}
void readingFile::readF()
{
	char T[2000];
	char TXX[5];
	///Модуль который должен находить сегодняшнию дату 
	FindDate();
	/// модуль который ищит временной отрезок по U
	FindTimePriod();
	char s;
	while (!file.get(s).eof())
	{
		file.getline( T, 2000, 'T' );
		file.getline( TXX, 5 ,' ');
		SelectionCipher( TXX );
	}
	TTXX.data_TTAA.sort();
	TTXX.data_TTBB.sort();
}
void readingFile::FindDate()
{	
	local_time st;
	int timeFile = 0;
	char s;
	//поиск  сегоднешней даты
	while (!file.get(s).eof())
	{
		char * str = new char[20];
		file >> str;
		if ( strchr(str,'&') )
		{
			char stringTime[10];
			file >> stringTime;
			timeFile = atoi(stringTime);
			if (st.wYear == timeFile/10000 && st.wMonth == (timeFile/100)%100 && st.wDay == timeFile%100 )
			{
				TTXX.data = timeFile;
				delete []str;
				break;
			}
		}
		
	}
}

void readingFile::FindTimePriod()
{
	local_time st;
	int timeFile = 0;
	char s;
	//поиск  сегоднешней даты
	while (!file.get(s).eof())
	{
		char * str = new char[20];
		file >> str;
		if ( strchr(str,'U') )
		{
			char stringTime[10];
			file >> stringTime;
			file >> stringTime;
			timeFile = atoi(stringTime);
			if ( st.wDay == timeFile/10000 && st.whour <= (((timeFile/100)%100)+5) &&  st.whour >= (((timeFile/100)%100)-5) )//  +-2
			{
				TTXX.time_period = timeFile;
				delete []str;
				break;
			}
		}
	}
}

void readingFile::SelectionCipher( char TXX[] )
{
	char * TTX = new char[1000] ;
	if ( strchr( TXX,'T')) 
		switch (TXX[1])
		{
		case  'A':
			{
				if (TXX[2] == 'A')
				{
					file.getline(TTX,1000,'=');
					TTXX.TTAA(TTX);
				}
				break;
			}
		case  'B':
			{
				if (TXX[2] == 'B')
				{					
					file.getline(TTX,1000,'=');
					TTXX.TTBB(TTX);
				}
				break;
			}
		case  'C':
			{
				file.getline(TTX,1000,'=');
				break;
			}
		case  'D':
			{
				file.getline(TTX,1000,'=');
				break;
			}
		default:
			{
				cout << "Not good" <<'\n';
				file.getline(TTX,1000,'=');
				cout << TTX << endl;
				break;
			}
		}
	delete []TTX;
}
readingFile::~readingFile( void )
{

}
