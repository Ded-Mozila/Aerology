#ifndef DB_STRUCTURE_TTBB_H
#define DB_STRUCTURE_TTBB_H 

#include <list>
#include "General_data.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////

struct Wind_Base 
{
	bool information;					// ������������� ������������� ������  
	int	number;							// ������������� �����, �����������, ��� ����� ���������� ������  � ����������� �����
	int pressure;						// �������� �� ������
	WIND wind;							// ������ �� �����
};

//////////////////////////////////////////////////////////////////////////

struct Temp_Base
{
	bool information;					// ������������� ������������� ������
	int	number;							// 
	int pressure;						// �������� �� ������
	TEMP_DEWPOINT info_temp;			// ���������� � ������ ����������� � ������� ����� ����
	
};

//////////////////////////////////////////////////////////////////////////

class TTBB_Database
{
public:
	bool information;					// ���������� ����������
	DATA_TIME memory;					// ������ � ������� � ���� ������� � + ��������������
	int number;			// ������ � ��������� ������� ����� � ������� 
	list<Temp_Base> level;				// ������ �� ����������� 
	list< list<Wind_Base> >level_wind;	// ������ �� �����
	bool operator< (const TTBB_Database & right) 
	{
		int m = right.number;
		int n = number;
		return n<m;
	} ;
};

class DateSurfase_TTBB
{
public:
	int date;
	int time;
	list<TTBB_Database> data_;
	bool operator< (const DateSurfase_TTBB & right) 
	{
		return date < right.date;
	} ;
};
#endif // !DB_STRUCTURE_TTBB_H