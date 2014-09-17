#include "readingFile.h"
#include <algorithm> 

// void DateSurfase_TTAA::FindReplicaToList()
// {

// }
readingFile::~readingFile()
{
}
readingFile::readingFile()
{
}

void readingFile::OpenFile_(  )
{
	TTXX.data = 0;
	app.stations.sort();
	list<int>::iterator i;
	for (i = app.stations.begin() ; i != app.stations.end(); ++i)
	{
		Station new_station;
		new_station.number = (*i);
		TTXX.time_00.push_back(new_station);
		TTXX.time_12.push_back(new_station);
	}
	file.open( app.outDirectory.c_str(), ios_base::in);
	if(!file)
	{
		perror("Error open file aeroindex.txt!");
		exit(1);
	}
	else
	{
		Read();
		cout << "Read Good \n";
		file.close();
		cout << '\n' << TTXX.time_00.size() <<  '\n' <<TTXX.time_12.size() <<'\n' ;

		TTXX.WriteFile( app.inDirectory );
	}
}


void readingFile::Read( void ) // Функция поиска данных
{
	local_time st;
	bool key_DATA = false;		// Отсутствие информации о Дате(нужна дата)
	bool thEnd = false;			// Окончание работы программы
	char s;						// Символ считывания (для обхода всего документа)
	while (!file.get(s).eof())	// Отбор проходит по символам & T U
	{
		switch(s)
		{
			case '&':// Указатель на Дату 
			{
				int timeFile;
				char * str = new char[20];
				file >> str;
				delete []str;
				char stringTime[10];
				file >> stringTime;
				timeFile = atoi(stringTime);
				if (st.wYear == timeFile/10000 && st.wMonth == (timeFile/100)%100 && st.wDay == timeFile%100 )
				{
					cout << timeFile;
					TTXX.data = timeFile;
					key_DATA = true;
				}
				break;
			}
			case 'T':// Указатель на Данные
			{
				if (key_DATA == true)
				{
					SelectionCipher();
				}
				else// Ускорение поиска -> пропуск ненужных данных
				{
					char * TTX = new char[2000] ;
					file.getline(TTX,2000,'=');
					delete []TTX;
				}
				break;
			}
			default://Пропуск данных (мусор)
			{
				break;
			}
		}
	}
}
void readingFile::SelectionCipher( void )
{
	char TXX[4];
	file.getline( TXX, 4 ,' ');
	char * TTX = new char[2000] ;
	if ( strchr( TXX,'T')) 
		switch (TXX[1])
		{
		case  'A':
			{
				file.getline(TTX,2000,'=');
				TTXX.TTAA(TTX);
				break;
			}
		case  'B':
			{
				file.getline(TTX,2000,'=');
				// TTXX.TTBB(TTX);
				break;
			}
		case  'C':
			{
				file.getline(TTX,1000,'=');
				// TTXX.TTCC(TTX);
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

// void readingFile::MadeNameAeroindex( void )
// {
// 	local_time st;
// 	stringstream ss;
// 	ss << dataFile_name << "/aeroindex";
// 	if(st.wDay < 10 ) ss << "0";
// 	ss << st.wDay ;
// 	if(st.wMonth < 10 ) ss << "0";
// 	ss << st.wMonth << st.wYear << "_";
// 	if ((TTXX.time_period/100)%100 == 0) ss << "0";
// 	ss << (TTXX.time_period/100)%100 << ".txt";
// 	ss >> dataFile_name;
// }