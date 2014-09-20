

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

char* recording_and_write::ReturnSurface( char* code, surface& info )
{
	//2. Определение температуры воздуха и дифицит точки росы
	//////////////////////////////////////////////////////////////////////////
	surface land = info;
	code = TempDewpoint(code, land.info_temp);
	//////////////////////////////////////////////////////////////////////////
	//3.. Определение направления  и скорости ветра
	code = Wind( code, land.wind );

	info = land;
	return code;
}

char* recording_and_write::Wind( char * code, WIND& new_wind )
{
	//3. Определенипе направление и скорость ветра
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
		}
	}
	else
	{
		int ddfff = strtol(code, &code, 10);
		new_wind.wind_direction = (ddfff / 1000) * 10 ; // Определение направление ветра
		if ((*code) =='/') 
		{
			new_wind.wind_direction = ddfff* 10 ; // Определение направление ветра
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
		}
	}	
	return code;
}

char * recording_and_write::Pressure( char * code, int & press)
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

char * recording_and_write::TempDewpoint( char * code, TEMP_DEWPOINT & new_info_temp )
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
			new_info_temp.dewpoint = DewPoint(DD);
		}
	}
	else
	{
		int TTTDD = strtol( code , &code , 10);
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
	return code;
}

char * recording_and_write::DateTime( char * code, DATA_TIME & new_data_tim )
{
	int month_time = strtol( code , &code , 10);
	if ((*code)=='/')
	{
		new_data_tim.date = month_time / 100;
		if( new_data_tim.date > 50) new_data_tim.date-=50; 
		new_data_tim.time = month_time % 100;
		new_data_tim.id_surface = '/';
		code++;
	} 
	else
	{
		new_data_tim.date = month_time / 1000;
		if( new_data_tim.date > 50) new_data_tim.date-=50; 
		new_data_tim.time = (month_time % 1000)/10;
		new_data_tim.id_surface = (month_time % 10) + '0';	
	}
	return code;	
}

char* recording_and_write::DistrictStation( char * code, int & new_number)
 {
	 int area = strtol( code , &code , 10);
	 new_number = area;
	 return code;
 }

char * recording_and_write::NumberHeight( char * code, NUMBER_HEIGHT & new_height, int & i)
{
	int PPhhh = strtol(code, &code, 10);
	//1. îïðåäåëåíèå ÷òî ýòî: äàííûå  î òðîïîïàóçå èëè äàííûå î âåòðå
	if((PPhhh/1000) == 88 || (PPhhh/1000) ==77)
	{
		i = STOP;
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
			if (i < 2)
			{
				if ( ((PPhhh % 1000) - 500) > 0 ) new_height.height = ((PPhhh % 1000) - 500 )*(-1);
				else new_height.height = PPhhh % 1000;
			}
			else new_height.height = PPhhh % 1000;
		}
	}
	return code;
}

char * recording_and_write::MaxWind(  char * code, surfaceWind& new_max_wind, int GGPPP )
{
	if ((GGPPP % 1000) != 999)
	{
		new_max_wind.data.information = true;
		code = Wind(code,new_max_wind.data.wind);
	} 
	else
	{
		new_max_wind.information = false;
		new_max_wind.data.information = false;
	}	
	return code;
}

WIND_SHIFT recording_and_write::WindShift( int GGPPP )
{
	WIND_SHIFT new_vertical;
	new_vertical.up_speed = (( GGPPP / 100 ) % 100);
	new_vertical.below_speed = ( GGPPP % 100 );
	return new_vertical;
}



