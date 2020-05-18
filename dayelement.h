#ifndef DAYELEMENT_H
#define DAYELEMENT_H
#include <string>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include<list>
#include "dayevent.h"
#include <QTime>

using namespace std;
class DayElement
{
private:
    QDate date;

    list<DayEvent> events;

public:
    DayElement(QDate date);

    ~DayElement(){};

    void addEvent(DayEvent event);

    void removeEvent(DayEvent event);

    DayEvent* findEvent(QTime timeStart, QTime timeEnd);

    int getDay()const{return this->date.day();}

    int getMonth()const{return this->date.month();}

    int getYear()const{return this->date.year();}

    QDate getDate()const{return this->date;}

    list<DayEvent> *getEvents(){return &this->events;}

    QString toString();

    bool operator==(const DayElement &other){
        return date.day() == other.date.day() && date.month() == other.date.month() && date.year() == other.date.year();
    }
    bool operator!=(const DayElement &other){
        return !(date.day() == other.date.day() && date.month() == other.date.month() && date.year() == other.date.year());
    }
    void operator=(const DayElement &other){
        this->date = other.date;
        list<DayEvent> newEvents(other.events);
        this->events = newEvents;
    }
};

#endif // DAYELEMENT_H
