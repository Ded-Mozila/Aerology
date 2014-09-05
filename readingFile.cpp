#include "readingFile.h"
#include <algorithm> 
void DateSurfase_TTAA::FindReplicaToList()
{

}

readingFile::readingFile(void)
{
}
void readingFile::OpenFile_(string name, string file_next)
{
	TTXX.data = 0 ; 
	file.open( name.c_str(), ios_base::in);
	if(!file)
	{
		perror("Error open file aeroindex.txt!");
		exit(1);
	}
	else
	{
		readF();
		file.close();
		//list<DateSurfase_TTAA>::iterator h = TTXX.data_TTAA.begin();
		//h->FindReplicaToList();
		TTXX.WriteFile(file_next);
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
	// задание файла dataFile
	local_time st;
	stringstream ss;
	ss << dataFile << "/aeroindex";
	if(st.wDay < 10 ) ss << "0";
	ss << st.wDay ;
	if(st.wMonth < 10 ) ss << "0";
	ss << st.wMonth << st.wYear << "_"; 
	if ((TTXX.time_period/100)%100 == 0) ss << "0"; 
	ss << (TTXX.time_period/100)%100 << ".txt";
	ss >> dataFile;
	char s;
	fstream FileData( dataFile.c_str(), ios::out);
	while (!file.get(s).eof())
	{
		file.getline( T, 2000, 'T' );
		file.getline( TXX, 5 ,' ');
		SelectionCipher( TXX ,FileData );
	}
	FileData.close();
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
			if ( st.wDay == timeFile/10000 && st.whour <= (((timeFile/100)%100)+3) &&  st.whour >= (((timeFile/100)%100)-2) )//  +-2
			{
				TTXX.time_period = timeFile;
				delete []str;
				break;
			}
		}
	}
}

void readingFile::SelectionCipher( char TXX[] , fstream& FileData)
{
	char * TTX = new char[2000] ;
	if ( strchr( TXX,'T')) 
		switch (TXX[1])
		{
		case  'A':
			{
				if (TXX[2] == 'A')
				{
					file.getline(TTX,2000,'=');
					FileData << "TTAA " << TTX << "\n\n";
					TTXX.TTAA(TTX);
					
				}
				break;
			}
		case  'B':
			{
				if (TXX[2] == 'B')
				{
					file.getline(TTX,2000,'=');
					FileData << "TTBB " << TTX << "\n\n";
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
