
#include "time.h"

#include <time.h>

BBString*  	bbCurrentDate(){
	time_t t;
	time( &t );
	char buff[256];

	tm tme;
	localtime_s(&tme, &t);


	strftime( buff,256,"%d %b %Y",&tme );
	return new BBString( buff );
}

BBString*  	bbCurrentTime(){
	time_t t;
	time( &t );
	char buff[256];

	tm tme;
	localtime_s(&tme, &t);


	strftime( buff,256,"%H:%M:%S",&tme );
	return new BBString( buff );
}

