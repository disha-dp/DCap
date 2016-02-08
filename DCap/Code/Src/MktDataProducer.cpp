/* 
 * File:   MktDataProducer.cpp
 * Author: dishapunjabi
 * 
 * Created on January 17, 2016, 3:07 AM
 */

#include "MktDataProducer.h"
#include<iostream>
#include "../../Api/TwsApiL0.h"
#include "../../Api/TwsApiDefs.h"
#include "Data/SharedStructures.h"

using namespace TwsApi;

bool EndOfHistoricalData	= false;
bool ErrorForRequest		= false;
extern SharedStructures ss;
int i=0;
std::string arr[]={"ITC","INFY","TATAMOTOR","HDFCBANK"};

// to use the Sleep function
#ifdef WIN32
	#include <windows.h>		// Sleep(), in miliseconds
	#include <process.h>
	#define CurrentThreadId GetCurrentThreadId
#else
	#include <unistd.h>			// usleep(), in microseconds
	#define Sleep( m ) usleep( m*1000 )
	#include <pthread.h>
	#define CurrentThreadId pthread_self
#endif

#define PrintProcessId printf("%ld  ", CurrentThreadId() )

//----------------------------------------------------------------------------
// MyEWrapper
//----------------------------------------------------------------------------
class MyEWrapper: public EWrapperL0
{
	public:

	MyEWrapper( bool CalledFromThread = true ) : EWrapperL0( CalledFromThread ) {}

	virtual void winError( const IBString& str, int lastError )
	{
		fprintf( stderr, "WinError: %d = %s\n", lastError, (const char*)str );
		ErrorForRequest = true;
	}

	virtual void error( const int id, const int errorCode, const IBString errorString )
	{
		fprintf( stderr, "Error for id=%d: %d = %s\n", id, errorCode, (const char*)errorString );
		ErrorForRequest = (id > 0);
		// id == -1 are 'system' messages, not for user requests
		// as a test, set year to 2010 in the reqHistoricalData
	}
/*
	virtual void historicalData( TickerId reqId, const IBString& date, double open, double high, double low, double close, int volume, int barCount, double WAP, int hasGaps )
	{
		if( IsEndOfHistoricalData(date) )
		{
			EndOfHistoricalData = true;
			return;
		}
               
                std::cout<<"Printing historical data:"<<std::endl;
		fprintf( stdout, "%10s, %5.3f, %5.3f, %5.3f, %5.3f, %7d\n", (const char*)date, open, high, low, close, volume );
	}
*/
virtual void historicalData( TickerId reqId, const IBString& date, double open, double high, double low, double close, int volume, int barCount, double WAP, int hasGaps )
	{
		if( IsEndOfHistoricalData(date) )
		{
			EndOfHistoricalData = true;
                        std::cout<<"marking the end of Historical data"<<std::endl;
			return;
		}
                //put in the shared struct
                cumulative_row r;
                //r.sym=sym_of[reqId];
                r.open=open; 
                r.high=high;
                r.low=low;
                r.close=close;
                r.wap=WAP;

                CumulativeData.insert(std::pair<std::string,cumulative_row>(sym_of[reqId],(r)));

                std::cout<<"Printing historical data for :"<<sym_of[reqId]<<" req id: "<<reqId<<std::endl;
		fprintf( stdout, "%10s, %5.3f, %5.3f, %5.3f, %5.3f, %7d\n", (const char*)date, open, high, low, close, volume );
	}
        
	virtual void tickPrice( TickerId tickerId, TickType field, double price, int canAutoExecute )
	{
		time_t		_t; time(&_t);
		struct tm*	_tm	= localtime( &_t );
		switch( field )
		{
		case TickTypes::Open:
		case TickTypes::High:
		case TickTypes::Low:
		case TickTypes::Close:
		case TickTypes::Last:
		printf
			( "TP: %4ld %02d:%02d:%02d %15s %5.3f\n"
			, tickerId
			, _tm->tm_hour, _tm->tm_min, _tm->tm_sec
			, *(TickTypes::ENUMS)field, price
			);
		default:{}
		}
	}

	virtual void tickSize( TickerId tickerId, TickType field, int size )
	{
		time_t		_t; time(&_t);
		struct tm*	_tm	= localtime( &_t );
		switch( field )
		{
		case TickTypes::LastSize:
                            {   printf
				( "TP: %4ld %02d:%02d:%02d %15s %5d\n"
				, tickerId
				, _tm->tm_hour, _tm->tm_min, _tm->tm_sec
				, *(TickTypes::ENUMS)field, size
				);
                        quote_row r;
                        //r.rNo=i++;
                        //r.sym=tickerId;
                        /*r.date=date;
                        r.open=open; 
                        r.vol=volume;*/
                        Quotes.insert(std::pair<std::string,quote_row>("ITC",r));
                        }
		default:{}
		}
	}

	virtual void tickString( TickerId tickerId, TickType tickType, const IBString& value )
	{
		time_t		_t; time(&_t);
		struct tm	_tm	= *localtime( &_t );
		time_t		_t1 = atol((const char*)value);
		struct tm	_tm1= *localtime( &_t1 );
		
		switch( tickType )
		{
		case TickTypes::LastTimestamp:
			printf
				( "TP: %4ld %02d:%02d:%02d %15s %02d:%02d:%02d\n"
				, tickerId
				, _tm.tm_hour, _tm.tm_min, _tm.tm_sec
				, *(TickTypes::ENUMS)tickType
				, _tm1.tm_hour, _tm1.tm_min, _tm1.tm_sec
				);
		default:{}
		}
	}   
    virtual void realtimeBar( TickerId reqId, long time, double open, double high, double low, double close, long volume, double wap, int count)
	{
                quote_row r;
                r.close=close;
                r.open=open;
                r.low=low;
                r.high=high;
                r.time=time;
                //r.sym=sym_of[reqId];
                r.volume=volume;
                r.wap=wap;
                r.count=count;
                Quotes.insert(std::pair<std::string,quote_row>("ITC",r));

                //Quotes.push_back(r);
                std::cout<<"Getting the real-time bar: high: "<<high<< " reqId is: "<< sym_of[reqId]<<std::endl;
	}
};
//----------------------------------------------------------------------------
// DUMP_DEFS
// Dumps the list of valid values for enum E
//----------------------------------------------------------------------------
// The iterator has a similar interface as the of the std::map
#define DUMP_DEFS( E ) \
	for( E::iterator i = E::begin(); i != E::end(); ++i ) { \
		printf( "\'%s\'\n", i->second ); }

//----------------------------------------------------------------------------
// ARGV
// Checks whether V is valid value for enum E
// Whn invalid, prints list of the valid values via DUMP_DEFS
//----------------------------------------------------------------------------
#define CHECK_VAR( E, V ) {\
	E::ENUMS e; \
	if( !(e *= V) ) { \
		printf("Error in argument '%s': value '%s' is not recognised.\nFollowing are valid values:\n", #E, V); \
		DUMP_DEFS( E ) \
		return 1; \
	} }

int MktDataProducer::GetMktData()
{
	Contract			C;
	C.symbol			= "ITC";
	C.secType			= *SecType::STK;		//"STK"
	C.currency			= "INR";
	C.exchange			= "NSE";	//"SMART";
	MyEWrapper	MW( false );	// no thread
	EClientL0*	EC = EClientL0::New( &MW );
	printf( "ClientVersion = %d\n", EC->clientVersion() );
	if( EC->eConnect( "", 7496, 100 ) )
	{
		EC->reqMktData( 100, C, "", false );

		time_t t1; time(&t1);
		time_t t2; time(&t2);
		while(time(&t2),(t2-t1) < 200)
		{
			EC->checkMessages();
		}
	}
	EC->eDisconnect();
	delete EC;
	{ PrintProcessId,printf( "Press return to end\n" ); char s[10]; gets(s); }
	return 0;
}

int MktDataProducer::GetRealtimeBars()
{
    for(int j=0;j<4;j++)
    {	Contract			C;
	C.symbol			= arr[j];
	C.secType			= *SecType::STK;		//"STK"
	C.currency			= "INR";
	C.exchange			= "NSE";	//"SMART";
        
	MyEWrapper	MW( false );	// no thread
	EClientL0*	EC = EClientL0::New( &MW );

	printf( "ClientVersion = %d\n", EC->clientVersion() );

	if( EC->eConnect( "", 7496, 100+j ) )
	{
		EC->reqRealTimeBars( 100, C, 5, "BID" ,0);

		time_t t1; time(&t1);
		time_t t2; time(&t2);
		while(1)//time(&t2),(t2-t1) < 20000)
		{
			EC->checkMessages();
		}
	}
    }
	//EC->eDisconnect();
	//delete EC;
	{ PrintProcessId,printf( "Press return to end\n" ); char s[10]; gets(s); }
	return 0;
}
/*
int MktDataProducer::GetMktHistData(int argc, const char* argv[])
{
    pthread_mutex_lock(&printf_mutex);
    std::cout<<"req mkt data here \n";
    pthread_mutex_unlock(&printf_mutex);
    
      

    Contract			C;
	C.symbol			= "ITC";
	C.secType			= *SecType::STK;	
	C.currency			= "INR";
	C.exchange			= "NSE";	
	
 
	int EDTY			= 2014;
	int EDTM			= 8;
	int EDTD			= 4;
	IBString DS			= DurationStr(1, *DurationHorizon::Months);
	IBString BSS		= *BarSizeSetting::_1_day;
	IBString WTS		= *WhatToShow::TRADES;

	if( argc > 1 )
	{
		if( argc < 8 )
		{
			printf	( "args are: symbol  end-date  duration  barsize  what-to-show.\n"
					  "     i.e. MSFT    20140804  10 D      1 day    TRADES\n"
					);
			return 1;
		}

		C.symbol		= argv[1];
		IBString EDT	= argv[2];
		if( EDT.size() != 8 )
		{
			printf	( "end-date should be 8 characters: YYYYMMDD\n"
					  "     i.e. 20140804\n"
					);
			return 1;
		}
		EDTY = atoi( EDT.substr(0,4).data() );
		EDTM = atoi( EDT.substr(4,2).data() );
		EDTD = atoi( EDT.substr(6,2).data() );

		CHECK_VAR( DurationHorizon, argv[4] ) // DurationHorizon
			DS		= DurationStr(atoi(argv[3]), argv[4] );

		IBString bss = argv[5]; bss = bss + " " + argv[6];
		CHECK_VAR( BarSizeSetting, (const char*)bss )	// BarSizeSetting
			BSS		= bss;

		CHECK_VAR( WhatToShow, argv[7] )	// WhatToShow
			WTS		= argv[7];
	}


	MyEWrapper	MW( false );
	EClientL0*	EC = EClientL0::New( &MW );

	if( EC->eConnect( "", 7496, 100 ) )
	{
		EC->reqHistoricalData
			( 20
			, C
			, EndDateTime(EDTY,EDTM,EDTD)	// Attention: for IB, this means last day will be 2013,02,19
			, DS							// DurationStr(1, *DurationHorizon::Months)
			, BSS							// *BarSizeSetting::_1_day
			, WTS							// *WhatToShow::TRADES
			, UseRTH::AllTradingData		// or use OnlyRegularTradingData
			, FormatDate::AsDate
//			, TagValueListSPtr()			// solved by default parameter in EClientL0
			);

		while( !EndOfHistoricalData && !ErrorForRequest )
			EC->checkMessages();
	}

	EC->eDisconnect();
        
        delete EC;

	return ErrorForRequest;
        
}
*/

int MktDataProducer::GetMktHistData(int argc, const char* argv[])
{
    EndOfHistoricalData=false; ErrorForRequest=false;
    
    pthread_mutex_lock(&printf_mutex);
    std::cout<<"req mkt data here \n";
    pthread_mutex_unlock(&printf_mutex);
    
      
        Contract			C;
	C.symbol			= "ITC";
	C.secType			= *SecType::STK;	
	C.currency			= "INR";
	C.exchange			= "NSE";	
	
	int EDTY			= 2016;
	int EDTM			= 2;
	int EDTD			= 3;
	IBString DS			= DurationStr(1, *DurationHorizon::Days);
	IBString BSS		= *BarSizeSetting::_1_day;
	IBString WTS		= *WhatToShow::TRADES;

	if( argc > 1 )
	{
		if( argc < 8 )
		{
			printf	( "args are: symbol  end-date  duration  barsize  what-to-show.\n"
					  "     i.e. MSFT    20140804  10 D      1 day    TRADES\n"
					);
			return 1;
		}

		C.symbol		= argv[1];
		IBString EDT	= argv[2];
		if( EDT.size() != 8 )
		{
			printf	( "end-date should be 8 characters: YYYYMMDD\n"
					  "     i.e. 20140804\n"
					);
			return 1;
		}
		EDTY = atoi( EDT.substr(0,4).data() );
		EDTM = atoi( EDT.substr(4,2).data() );
		EDTD = atoi( EDT.substr(6,2).data() );

		CHECK_VAR( DurationHorizon, argv[4] ) // DurationHorizon
			DS		= DurationStr(atoi(argv[3]), argv[4] );

		IBString bss = argv[5]; bss = bss + " " + argv[6];
		CHECK_VAR( BarSizeSetting, (const char*)bss )	// BarSizeSetting
			BSS		= bss;

		CHECK_VAR( WhatToShow, argv[7] )	// WhatToShow
			WTS		= argv[7];
	}

	MyEWrapper	MW( false );
	EClientL0*	EC = EClientL0::New( &MW );
        int i=0;
    while(i++<2000)//set to particular value ??????
    {
        for(int j=0;j<4;j++)
     {
        EndOfHistoricalData=false; ErrorForRequest=false;
	if(!EC->IsConnected())
        {
            (EC->eConnect( "", 7496, 100 ) );
	}
        C.symbol=arr[j];
            //std::cout<<"requesting mkt data : "<<std::endl;
		EC->reqHistoricalData
			( 100+j
			, C
			, EndDateTime(2016,02,03)	// Attention: for IB, this means last day will be 2013,02,19
			, DS							// DurationStr(1, *DurationHorizon::Months)
			, BSS							// *BarSizeSetting::_1_day
			, WTS							// *WhatToShow::TRADES
			, UseRTH::AllTradingData		// or use OnlyRegularTradingData
			, FormatDate::AsDate
//			, TagValueListSPtr()			// solved by default parameter in EClientL0
			);

		while( !EndOfHistoricalData && !ErrorForRequest )
			EC->checkMessages();
                //std::cout<<"came out of the while waiting for historical data : "<<std::endl;

        }
                        sleep(15);

    }
	EC->eDisconnect();
        
        delete EC;

	return ErrorForRequest;
}
MktDataProducer::MktDataProducer() {
    //int r= GetMktHistData(0,NULL);
    //GetMktData();
    GetMktHistData(0,NULL);
   GetRealtimeBars();

}

MktDataProducer::MktDataProducer(const MktDataProducer& orig) {
    
}

MktDataProducer::~MktDataProducer() {
}

