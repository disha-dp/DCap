/* 
 * File:   Main.cpp
 * Author: dishapunjabi
 *
 * Created on January 16, 2016, 6:55 PM
 */

#include <cstdlib>
#include<iostream>
#include<pthread.h>
#include "Data/SharedStructures.h"
#include "Strategies/Strat1.cpp"
#include "MktDataProducer.cpp"


/*
using namespace std;

*/

//============================================================================
// TwsApi Test
//============================================================================
#include "../../Api/TwsApiL0.h"
#include "../../Api/TwsApiDefs.h"
using namespace TwsApi;



void * MktData(void* threadid)
{
    MktDataProducer *mkt=new MktDataProducer;
    return NULL;
}

void * Start_Strat1(void* threadid)
{
    Strat1 *st1=new Strat1;
    return NULL;
}
void InitSymNumbers()
{
    sym_of[100]="ITC";

}
//----------------------------------------------------------------------------
// main
//----------------------------------------------------------------------------
int main( int argc, const char* argv[] )
{
    InitSymNumbers();
    //schedule strat 1 to start after 15 sec
    pthread_t strat1_th;
    int res=pthread_create(&strat1_th,NULL, Start_Strat1,NULL);
    if(res)
    {
        std::cout<<"Error from pthread create \n";
    }
    pthread_t mktData_th;
     int res2=pthread_create(&mktData_th,NULL, MktData,NULL);
    if(res2)
    {
        std::cout<<"Error from pthread create \n";
    }
        //print the shared structure
        //std::cout<<"Quotes DS frmo main:"<<std::endl;
        for(int j=0;j<i;j++)
        {
            std::cout<<" "<<Quotes[j].sym<< " "<<Quotes[j].open <<" "<<Quotes[j].volume<<" \n" ;
        }
        //end of printing
	Sleep(30);
        pthread_join(strat1_th,NULL);        
        pthread_join(mktData_th,NULL);

        
}


