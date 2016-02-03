/* 
 * File:   MktDataProducer.h
 * Author: dishapunjabi
 *
 * Created on January 17, 2016, 3:07 AM
 */

#ifndef MKTDATAPRODUCER_H
#define	MKTDATAPRODUCER_H


class MktDataProducer {
public:
    MktDataProducer();
    MktDataProducer(const MktDataProducer& orig);
    virtual ~MktDataProducer();
     int GetMktHistData(int a, const char* b[]);
     int GetMktData();
     int GetRealtimeBars();
private:

};

#endif	/* MKTDATAPRODUCER_H */

