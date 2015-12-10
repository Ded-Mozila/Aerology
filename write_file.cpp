#include "WriteFile.h"

recording_and_write::recording_and_write( void )
{
	
}

recording_and_write::~recording_and_write(void)
{
}

double recording_and_write::DewPoint( int TTTDD )
{
	double dewp = TTTDD % 100;
	if (( dewp ) <= 50)
	{
		dewp = dewp / 10.0;
	} 
	else
	{
		dewp = dewp  - 50.0;
	}
	return dewp;
}

double recording_and_write::DefinitionTenthTemp( int m )
{
	int n = m % 10;
	double temp_ =  m / 10;
	if ( (( n % 2 ) == 0) || (n == 0) )
	{
		temp_+=( n * 0.1 );			// Åñëè òåìïåðàòóðà ÷èëî ÷åòíîå èëè = 0
	} 
	else
	{
		temp_+=( n * 0.1 );							// Åñëè ÷èñëî íå÷åòíîå							
		temp_*=-1;
	}
	return temp_;
}

double recording_and_write::Temp( bool presence_dewpoint,const int TTTDD )
{
	int m = 0 ;
	double temp_ = 0;
	if (presence_dewpoint == true  )
	{
		m = TTTDD / 100 ;
		temp_ = DefinitionTenthTemp(m);
	} 
	else
	{
		m = TTTDD;
		temp_ = DefinitionTenthTemp(m);
	}
	return temp_;
}

char* recording_and_write::ReturnSurface( char* code, surface& info , int& i )
{
	surface trop;		// новая тропопауза
	surfaceWind wind77;	// Максимальный ветер
	WIND_SHIFT shift;	// Вертикальный сдвиг
	//2. Определение температуры воздуха и дифицит точки росы
	//////////////////////////////////////////////////////////////////////////
	surface land = info;
	code = TempDewpoint(code,trop,wind77,shift, land.info_temp , i);
	//////////////////////////////////////////////////////////////////////////
	//3.. Определение направления  и скорости ветра
	code = Wind(code,trop,wind77,shift, land.wind,land.wind_node, i);
	info = land;
	return code;
}

char* recording_and_write::Wind(char * code,surface& trop, surfaceWind& wind77,WIND_SHIFT& shift, WIND& new_wind , bool node,  int& i)
{
	if(*(code+1) =='/') //Проверка на присутствие скороти ветра
	{
		code +=3;
		new_wind.wind_direction = 999;
		if ((*(code)) =='/') 
		{
			code += 3;
			new_wind.wind_speed = 999;			// Отсутствие данных о направлении ветра
		} 
		else
		{
			new_wind.wind_speed = strtol (code, &code, 10);
			if(node == true)new_wind.wind_speed *=0.514;
		}
	}
	else
	{

		int ddfff = strtol(code, &code, 10);
		if((ddfff/1000) == 88 || (ddfff/1000) ==77 || (ddfff/1000) == 66 )
		{
			//Получение данных о тропопаузе
			code = endGroup(ddfff,code,trop,wind77,shift,i);
		}
		else
		{
			new_wind.wind_direction = (ddfff / 1000) * 10 ; // Определение направление ветра
			if ((*code) =='/') 
			{
				new_wind.wind_direction = ddfff * 10 ; // Определение направление ветра
				code += 3;
				new_wind.wind_speed = 999;			// Отсутствие данных о направлении ветра
			} 
			else
			{
				new_wind.wind_direction = (ddfff / 1000) * 10 ; // Определение направление ветра
				int fff = ddfff % 1000;
				if(( fff - 500 ) >= 0)
				{
					fff -=500;
					new_wind.wind_direction +=5;
				}
				new_wind.wind_speed = fff;
				if(node == true)new_wind.wind_speed *=0.514;
			}
		}
	}	
	return code;
}
WIND recording_and_write::Wind(string code, bool node)
{
	WIND new_wind;
	int ddfff = atoi(code.c_str());
	if(code[0] !='/') //Проверка на присутствие скороти ветра
	{
		if (code[2] !='/') 
		{
			new_wind.wind_direction = (ddfff / 1000) * 10 ; // Определение направление ветра
			int fff = ddfff % 1000;
			if(( fff - 500 ) >= 0)
			{
				fff -=500;
				new_wind.wind_direction +=5;
			}
			new_wind.wind_speed = fff;
		}
		else
		{
			new_wind.wind_direction = ddfff * 10 ; // Определение направление ветра
			if( code[2] == '5')
			{
				new_wind.wind_direction +=5;
			}
		}
	}
	else
	{
		if (code[2] !='/') 
		{
			int fff = ddfff % 1000;
			new_wind.wind_speed = fff;
		}
	}
	if(node == true && new_wind.wind_speed != 999)new_wind.wind_speed *=0.514;	
	return new_wind;
}
char * recording_and_write::Pressure( char * code, int & press )
{
	int pressure_ = strtol( code , &code , 10);
	pressure_ %= 1000;
	if (pressure_ < 100)
	{
		pressure_ = pressure_ + 1000;
	} 
	press = pressure_;
	return code;
}

int recording_and_write::Pressure(string code)
{
	int pressure_ = atoi(code.c_str());
	pressure_ %= 1000;
	if (pressure_ < 100)
	{
		pressure_ = pressure_ + 1000;
	} 
	return pressure_;
}

char * recording_and_write::TempDewpoint( char * code, surface& trop, surfaceWind& wind77,WIND_SHIFT& shift, TEMP_DEWPOINT & new_info_temp , int& i)
{
	if(*(code+1) == '/')
	{
		code +=4;
		new_info_temp.temp = 999;
		if(*code == '/')
		{
			code += 2;
			new_info_temp.dewpoint = 999;
		}			
		else
		{
			int DD = strtol( code , &code , 10);
			if((DD/1000) == 88 || (DD/1000) ==77 || (DD/1000) == 66 )
			{
				//Получение данных о тропопаузе
				code = endGroup(DD,code,trop,wind77,shift,i);
				return code;
			}
			new_info_temp.dewpoint = DewPoint(DD);
		}
	}
	else
	{
		int TTTDD = strtol( code , &code , 10);
		if((TTTDD/1000) == 88 || (TTTDD/1000) ==77 || (TTTDD/1000) == 66 )
		{
			//Получение данных о тропопаузе
			code = endGroup(TTTDD,code,trop,wind77,shift,i);
		}
		else
		{
			if(*code == '/')
			{
				code += 2;
				new_info_temp.dewpoint = 999;
				new_info_temp.temp = Temp( false, TTTDD );
			}
			else
			{
				new_info_temp.dewpoint = DewPoint(TTTDD);
				new_info_temp.temp = Temp( true, TTTDD );
			}
		}
	}	
	return code;
}

TEMP_DEWPOINT recording_and_write::TempDewpoint(string code)
{
	TEMP_DEWPOINT new_info_temp;
	if(code != "/////")
	{
		if(code.at(3) != '/')
		{
			int DD = atoi(code.c_str());
			new_info_temp.dewpoint = DewPoint(DD);
			if(code.at(0) != '/')
				new_info_temp.temp = Temp( true, DD );
		}
		else
		{
			int TTT = atoi(code.c_str());
			new_info_temp.temp = Temp( false, TTT );
		}
	}
	return new_info_temp;
}

char * recording_and_write::DateTime( char * code, bool& node, DATA_TIME & new_data_tim )
{
	int month_time = strtol( code , &code , 10);
	if ((*code)=='/')// || month_time < sizeCout)
	{
		new_data_tim.date = month_time / 100;
		if( new_data_tim.date > 50) 
			{
				node = true;
				new_data_tim.date-=50;
			}
		new_data_tim.time = month_time % 100;
		new_data_tim.id_surface = '/';
		code++;
	} 
	else
	{
		new_data_tim.date = month_time / 1000;
		if( new_data_tim.date > 50) 
			{
				node = true;
				new_data_tim.date-=50; 
			}
		new_data_tim.time = (month_time % 1000)/10;
		new_data_tim.id_surface = (month_time % 10) + '0';	
	}
	return code;	
}
//Новая версия программы
DATA_TIME recording_and_write::DateTime( string code, bool& node)
{
	int month_time = atoi(code.c_str());
	//cout << "Time " << code <<" -- " << month_time << " " << code[4] <<  "\n";
	DATA_TIME now; 
	if (code[4] == '/')
	{
		now.date = month_time / 100;
		if( now.date > 50) 
			{
				node = true;
				now.date-=50;
			}
		now.time = month_time % 100;
		now.id_surface = code[4];
	} 
	else
	{
		now.date = month_time / 1000;
		if( now.date > 50) 
			{
				node = true;
				now.date-=50; 
			}
		now.time = (month_time % 1000)/10;
		now.id_surface = (month_time % 10) + '0';	
	}
	return now;	
}

char* recording_and_write::DistrictStation( char * code, int & new_number)
{
	 int area = strtol( code , &code , 10);
	 new_number = area;
	 return code;
}

int recording_and_write::DistrictStation(string code)
{
	return atoi(code.c_str());
}

char * recording_and_write::NumberHeight( char * code, surface& trop, surfaceWind& wind77,WIND_SHIFT& shift, NUMBER_HEIGHT & new_height, int & i)
{
	int PPhhh = strtol(code, &code, 10);
	//1. îïðåäåëåíèå ÷òî ýòî: äàííûå  î òðîïîïàóçå èëè äàííûå î âåòðå
	if(PPhhh == 31313 || (PPhhh/1000) == 88 || (PPhhh/1000) ==77 || (PPhhh/1000) == 66 )
	{
		//Получение данных о тропопаузе
		if (PPhhh == 31313 ) {
			i = 31313;
			return code;
		}
		else
		code = endGroup(PPhhh,code,trop,wind77,shift,i);
	}
	else
	{
		if((*code)=='/')
		{
			new_height.number = PPhhh;
			new_height.height = 999;
			code += 3;
		}
		else
		{
			new_height.number = PPhhh / 1000;
			new_height.height = PPhhh % 1000;
		}
	}
	return code;
}

NUMBER_HEIGHT recording_and_write::NumberHeight(string code)
{
	NUMBER_HEIGHT data;
	int PPhhh = atoi(code.c_str());
	if(code[2] == '/')
	{
		data.number = PPhhh;
	}
	else
	{
		data.number = PPhhh / 1000;
		data.height = PPhhh % 1000;
	}
	return data;
}

char * recording_and_write::endGroup(const int PPhhh,char * code, surface& trop, surfaceWind& wind77,WIND_SHIFT& shift, int & i)
{
	
	if((PPhhh/1000) == 88  ) 
	{
		i = 888;//есть тропопауза
		if(PPhhh%1000 != 999)
		{
			trop.pressure = PPhhh%1000;
			trop.information = true;
			code = ReturnSurface( code , trop ,i );
		}
		else trop.information = false;
	}
	if((PPhhh/1000) ==77 )
	{
		i = 777;
		if(PPhhh%1000 != 999)
		{
			wind77.information = true;
			wind77.point = 77;
			wind77.data.pressure = PPhhh%1000;
			code = Wind(code,trop,wind77,shift,wind77.data.wind,false, i );
			int vb = strtol(code, &code, 10);
			if((vb/10000) == 4 && ((vb/1000)%10 >= 0 ))
			{
				wind77.shift.information = true;
				wind77.shift.up_speed = (vb/100)%100;
				wind77.shift.below_speed = vb%100;
			}
			else code-=5;

		}
		else wind77.information = false;
	}
	if((PPhhh/1000) ==66 )
	{
		i = 666;
		if(PPhhh%1000 != 999)
		{
			wind77.information = true;
			wind77.point = 66;
			wind77.data.pressure = PPhhh%1000;
			code = Wind(code,trop,wind77,shift,wind77.data.wind,false, i );
			int vb = strtol(code, &code, 10);
			if((vb/10000) == 4 && ((vb/1000)%10 >= 0 ))
			{
				wind77.shift.information = true;
				wind77.shift.up_speed = (vb/100)%100;
				wind77.shift.below_speed = vb%100;
			}
			else code-=5;
		}
		else wind77.information = false;
	}
	// if((PPhhh/10000) == 4 && ((PPhhh/1000)%10 >0 || (PPhhh/1000) == 88 || (PPhhh/1000) ==77 || (PPhhh/1000) == 66))
	// {
	// 	i == 444;
	// 	shift.up_speed = (PPhhh/100)%100;
	// 	shift.below_speed = PPhhh%100;
	// }
	return code;
}
char * recording_and_write::MaxWind(  char * code, surfaceWind& new_max_wind, int GGPPP )
{
	// if ((GGPPP % 1000) != 999)
	// {
	// 	new_max_wind.data.information = true;
	// 	int i=0;
	// 	code = Wind(code,new_max_wind.data.wind, i );
	// } 
	// else
	// {
	// 	new_max_wind.information = false;
	// 	new_max_wind.data.information = false;
	// }	
	// return code;
}

WIND_SHIFT recording_and_write::WindShift( int GGPPP )
{
	WIND_SHIFT new_vertical;
	new_vertical.up_speed = (( GGPPP / 100 ) % 100);
	new_vertical.below_speed = ( GGPPP % 100 );
	return new_vertical;
}



