#include "readingFile.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <algorithm> 
readingFile::~readingFile()
{
}
readingFile::readingFile()
{
}
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
    	if(string(dirp->d_name) != "." && string(dirp->d_name) != ".." )
        	files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
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
		cout << "Read_Good \n";
		file.close();
		TTXX.WriteFile( app.inDirectory );
		outDataFile();
	}
}
void readingFile::OpenDirPeriod(local_time start, local_time end)
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
	//Открытие дериктории и открытие под файлов
	string namedir =  app.outDirectory;
	string nameOut = app.outDirectory;
	cout << namedir<< endl;
	vector<string> files = vector<string>();
	getdir(namedir.c_str(),files);

    for (unsigned int i = 0;i < files.size();i++) {
    	string addresFile = namedir;
    	string nameOut_year = nameOut;
        if (start.wYear <= atoi(files[i].c_str()) && atoi(files[i].c_str()) <= end.wYear)
        {
        	vector<string> files_months = vector<string>();
        	addresFile += files[i] + "/"; 
        	nameOut_year += files[i] + "/";
        	getdir(addresFile.c_str(),files_months);
        	std::sort(files_months.begin(),files_months.end());
			for (unsigned int j = 0;j < files_months.size();j++) {
				if (start.wMonth <= atoi(files_months[j].c_str()) )
        		{	
	        		string addresFile_day = addresFile + files_months[j] + "/";
	        		string nameOut_year_month = nameOut_year + files_months[j] + "/";
					vector<string> files_day = vector<string>();
					getdir(addresFile_day.c_str(),files_day );
					std::sort(files_day.begin(),files_day.end());
					for (unsigned int k = 0;k < files_day.size();k++) {
						string addresFile_day_name = addresFile_day +  files_day[k];
						cout << "Start_" + addresFile_day_name  + endl;
						file.open( addresFile_day_name.c_str(), ios_base::in);
						if(!file)
						{
							perror("Error open file aeroindex.txt!");
							exit(1);
						}
						else
						{
							Read(); 
							file.close();
							TTXX.WriteFileClouds(addresFile_day);
							TTXX.WriteFile( app.inDirectory );
							cout << "End_" + addresFile_day_name + endl;
							//outDataFile();
						}	
					}
					files_day.clear();
				}
			}
			files_months.clear();
        }
    }
    files.clear();
	//perror("Error open file aeroindex.txt!");
	exit(1);
	//посмотреть как работать с дерикториями вновь
	file.open( app.outDirectory.c_str(), ios_base::in);
	if(!file)
	{
		perror("Error open file aeroindex.txt!");
		exit(1);
	}
	else
	{
		Read(); 
		cout << "Read_Good \n";
		file.close();
		TTXX.WriteFile( app.inDirectory );
		outDataFile();
	}
}

void readingFile::Read( void ) // Функция поиска данных
{
	local_time st;
	bool key_DATA = false;		// Отсутствие информации о Дате(нужна дата)
	//cout << "dctulf true" << endl;
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
					cout << timeFile << endl;
					TTXX.data = timeFile;
					key_DATA = true;
				}
				//условие проверки другого дня
				else key_DATA = false; ///ИПРАВИТЬ!!!
				break;
			}
			case 'T':// Указатель на Данные
			{
					file.get(s);
					if(s == 'T')
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
					}

					break;
			}
		}
	}
}
void readingFile::SelectionCipher( void ) //
{
	try
	{
		char TXX[4];
		file.getline( TXX, 4 ,' ');
		char * code = new char[2000] ;
		switch (TXX[0])
		{
		case  'A':
			{
				file.getline(code,2000,'=');
				string cloneCode(code); 
				code = TTXX.deleteEndl(code);				//Замена '\n' на пробел
				if(FindDefectCipher(code) == false)
					TTXX.TTAA(code,cloneCode);
				break;
			}
		case  'B':
			{
				file.getline(code,2000,'=');
				string cloneCode(code); 
				code = TTXX.deleteEndl(code);				//Замена '\n' на пробел
				if(FindDefectCipher(code) == false)
					TTXX.TTBB(code,cloneCode, 1);
				break;
			}
		case  'C':
			{
				file.getline(code,2000,'=');
				string cloneCode(code); 
				code = TTXX.deleteEndl(code);				//Замена '\n' на пробел
				if(FindDefectCipher(code) == false)
					TTXX.TTCC(code,cloneCode);
				break;
			}
		case  'D':
			{
				file.getline(code,2000,'=');
				string cloneCode(code); 
				code = TTXX.deleteEndl(code);				//Замена '\n' на пробел
				if(FindDefectCipher(code) == false)
					TTXX.TTBB(code,cloneCode, 2);
				break;
			}
		default:
			{
				cout << "Read error" <<'\n';
				file.getline(code,2000,'=');
				// cout << code << endl;
				break;
			}
		}
		delete []code;
	}
	catch(...)
	{
		cout << "Error! in listing file" << endl;
	}
}
bool readingFile::FindDefectCipher(char * code)
{
	bool key = false;
	int lenStr = strlen(code);
	for (int i = 0; i < lenStr; i++)
	{
		switch(code[i])
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case ' ':
			case '/':
			case 'N':
			case 'I':
			case 'T':
			case 'L':
			case '\0':
			case '\n':
			{
				break;
			}
			default:
			{
				cout << "Found a broken cipher!  :"<<  code << '\n';
				key = true;
				break;
			}
		}
			if(key == true) break;
	}	
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
	return str;
}

void readingFile::outDataFile( void )
{
	outInfoFileDecodePeriod(0);
	outInfoFileDecodePeriod(12);
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
	for ( int k = 0; k <4 ;++k )
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
				case 3: // TTDD
				{
					if ((*i).info == true && (*i).TTDD.information == true)
					{
						dataFile << "TTDD " << (*i).TTDD.code_ << "=\n";
					}
					break;
				}
			}
		}
		dataFile << "-----------------------------------------------------------------------\n";
	}
	dataFile.close();
}



