/* 
 * File:   SharedStructures.h
 * Author: dishapunjabi
 *
 * Created on January 16, 2016, 6:41 PM
 */

#include<vector>
#include<iterator>
#include<list>
#include<string>
#include<map>
#ifndef SHAREDSTRUCTURES_H
#define	SHAREDSTRUCTURES_H
std::map<int,std::string> sym_of;
struct cumulative_row{
    //std::string sym;
    double open;
    double high;
    double low;
    double close;
    long volume;
    long barCount;
    double wap;
};
    
struct quote_row{
    long time;
    double open;
    double high;
    double low;
    double close;
    long volume;
    double wap;
    int count;
    long rNo;
    //std::string sym;
    std::string date;
};

    std::map<std::string,cumulative_row> CumulativeData; 
    std::map<std::string,quote_row> Quotes; 

class SharedStructures {
    

public:

    SharedStructures();
    SharedStructures(const SharedStructures& orig);
    virtual ~SharedStructures();
private:

};

#endif	/* SHAREDSTRUCTURES_H */

