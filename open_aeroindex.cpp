#include "readingFile.h"
#include <algorithm> 

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
	for (i = app.stations.begin(); i != app.stations.end(); ++i)
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
		TTXX.WriteFile( app.inDirectory );
		outDataFile();
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
	char * code = new char[2000] ;
	if ( strchr( TXX,'T')) 
		switch (TXX[1])
		{
		case  'A':
			{
				file.getline(code,2000,'=');
				code = TTXX.deleteEndl(code);				//Замена '\n' на пробел
				if(FindDefectCipher(code) == false)
					TTXX.TTAA(code);
				break;
			}
		case  'B':
			{
				file.getline(code,2000,'=');
				code = TTXX.deleteEndl(code);				//Замена '\n' на пробел
				if(FindDefectCipher(code) == false)
					TTXX.TTBB(code);
				break;
			}
		case  'C':
			{
				file.getline(code,2000,'=');
				code = TTXX.deleteEndl(code);				//Замена '\n' на пробел
				if(FindDefectCipher(code) == false)
					TTXX.TTCC(code);
				break;
			}
		case  'D':
			{
				file.getline(code,2000,'=');
				code = TTXX.deleteEndl(code);				//Замена '\n' на пробел
				if(FindDefectCipher(code) == false)
				{}
				break;
			}
		default:
			{
				// cout << "Not good" <<'\n';
				// cout << code << endl;
				break;
			}
		}
	delete []code;
}
bool readingFile::FindDefectCipher(char * code)
{
	bool key = false;
	int lenStr = strlen(code);
	// for (int i = 0; i < lenStr; i++)
	// {
	// 	switch(code[i])
	// 	{
	// 		case '0':
	// 		{
	// 			break;
	// 		}
	// 		case '1':
	// 		{
	// 			break;
	// 		}
	// 		case '2':
	// 		{
	// 			break;
	// 		}
	// 		case '3':
	// 		{
	// 			break;
	// 		}
	// 		case '4':
	// 		{
	// 			break;
	// 		}
	// 		case '5':
	// 		{
	// 			break;
	// 		}
	// 		case '6':
	// 		{
	// 			break;
	// 		}
	// 		case '7':
	// 		{
	// 			break;
	// 		}
	// 		case '8':
	// 		{
	// 			break;
	// 		}
	// 		case '9':
	// 		{
	// 			break;
	// 		}
	// 		case ' ':
	// 		{
	// 			break;
	// 		}
	// 		case '/':
	// 		{
	// 			break;
	// 		}
	// 		case 'N':
	// 		{
	// 			break;
	// 		}
	// 		case 'I':
	// 		{
	// 			break;
	// 		}
	// 		case 'L':
	// 		{
	// 			break;
	// 		}
	// 		case '\0':
	// 		{
	// 			break;
	// 		}
	// 		case '\n':
	// 		{
	// 			break;
	// 		}
	// 		default:
	// 		{
	// 			// cout << code[i] << '\n';
	// 			key = true;
	// 			break;
	// 		}
	// 	}
	// 		if(key == true) break;
	// }	
	return key;
}


string readingFile::MadeNameAeroindex( int period, const string _file )
{
	local_time st;
	stringstream ss;
	string str;
	ss << _file << "/aeroindex"<< setfill ('0')<< setw (2)<< st.wDay \
	<< setw (2)  << st.wMonth << setw (4) << st.wYear << "_"\
	<< setw (2) << period << ".txt";
	ss >> str;
	cout << str.c_str() << '\n';
	return str;
}

void readingFile::outInfoFileDecodePeriod(int period)
{
	dataFile.open((MadeNameAeroindex(period, app.dataDirectory)).c_str(), ios_base::out);
	list<Station>::iterator i;
	list<Station>::iterator i_begin;
	list<Station>::iterator i_end;
		if( period == 0 )			// 00
		{
			i_begin = TTXX.time_00.begin();
			i_end = TTXX.time_00.end();
		}
		if( period == 12 )		// 12
		{
			i_begin = TTXX.time_12.begin();
			i_end = TTXX.time_12.end();
		}
	for ( int k = 0; k <3 ;++k )
	{
		for ( i =i_begin; i != i_end; ++i )
		{
			switch(k)
			{
				case 0: // TTAA
				{
					if ((*i).info == true && (*i).TTAA.information == true)
					{
						dataFile << "TTAA " << (*i).TTAA.code_ << "=\n";
					}
					break;
				}
				case 1: // TTBB
				{	
					if ((*i).info == true && (*i).TTBB.information == true)
					{
						dataFile << "TTBB " << (*i).TTBB.code_ << "=\n";
					}
					break;
				}
				case 2: // TTCC
				{
					if ((*i).info == true && (*i).TTCC.information == true)
					{
						dataFile << "TTCC " << (*i).TTCC.code_ << "=\n";
					}
					break;
				}
			}
		}
		dataFile << "-----------------------------------------------------------------------\n";
	}
	dataFile.close();
}

void readingFile::outDataFile( void )
{
	outInfoFileDecodePeriod(0);
	outInfoFileDecodePeriod(12);
}


