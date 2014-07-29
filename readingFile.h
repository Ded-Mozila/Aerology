#ifndef READINGFILE_H
#define READINGFILE_H 


#include "WriteFile.h"
class readingFile
{
public:

	fstream file; 								// Ôàéë 
	recording_and_write  TTXX;					// Êëàññ ðàçêîäèðîâàíèÿ è çàïèñè
	readingFile(void);							// Ïî óìîë÷àíèþ
	void OpenFile_( string name, string file_next );
	void readF();
	void FindDate();
	void FindTimePriod();
	void SelectionCipher( char TXX[] );
	~readingFile(void);	
};

#endif	// !READINGFILE_H 

