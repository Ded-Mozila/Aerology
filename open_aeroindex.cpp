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
	TTXX.data = 0;
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
		TTXX.WriteFile(file_next);
	}
}

void readingFile::readF()
{
	char T[2000];
	char TXX[5];
	char s;
	while (!file.get(s).eof())							// отбор проходит по символам & T U
	{
		switch(s)
		{
			case '&':// Указатель на Дату 
			{

				break;
			}
			case 'U':// Указательна Период
			{
				
				break;
			}
			case 'T':// Указатель на Данные
			{
				
				break;
			}
			default://Пропуск данных (мусор)
			{
				break;
			}
		}
	}
	fstream FileData( dataFile.c_str(), ios::out);		// Open aeroindex.txt
	while (!file.get(s).eof())
	{
		file.getline( T, 2000, 'T' );
		file.getline( TXX, 5 ,' ');
		SelectionCipher( TXX ,FileData );
	}
	FileData.close();
	TTXX.data_TTAA.sort();
	TTXX.data_TTBB.sort();
	TTXX.data_TTCC.sort();
}

void MadeNameAeroindex()
{
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
}