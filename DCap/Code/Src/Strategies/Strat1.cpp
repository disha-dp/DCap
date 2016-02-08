/* 
 * File:   Strat1.cpp
 * Author: dishapunjabi
 * 
 * Created on January 16, 2016, 7:56 PM
 */

#include "Strat1.h"
#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include <vector>

pthread_mutex_t printf_mutex=PTHREAD_MUTEX_INITIALIZER;

Strat1::Strat1() {
    pthread_mutex_lock(&printf_mutex);
    std::cout<<"will start after 15 sec, sleeping now\n";
    pthread_mutex_unlock(&printf_mutex);
    sleep(5);
    
    pthread_mutex_lock(&printf_mutex);
    std::cout<<"started after 5 sec \n";
    pthread_mutex_unlock(&printf_mutex);
    
    Start_Work();
}

Strat1::Strat1(const Strat1& orig) {
}

Strat1::~Strat1() {
}
void Strat1::Start_Work()
{
    std::cout<<" This is strategy 1 working\n";
    int i=0;
    while(i<50)
    {

        pthread_mutex_lock(&printf_mutex);
        for(int j=0;j<2;j++)
        {
            //since I am just reading, i will not acquire mutex
            //if required, in future, we will implement semaphore for reader-writer
            //std::cout<<" "<<Quotes[j].sym<< " "<<Quotes[j].open <<" "<<Quotes[j].volume<<" \n" ;
            std::cout<<" bwaahh "<<std::endl;
        }
            pthread_mutex_unlock(&printf_mutex);

        sleep(20);
    }
}