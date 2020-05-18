#include "dayelement.h"

DayElement::DayElement(QDate date):date(date){

}
void DayElement::addEvent(DayEvent event){
     events.push_back(event);
}
void DayElement::removeEvent(DayEvent event){
    this->events.remove(event);
}

DayEvent* DayElement::findEvent(QTime timeStart, QTime timeEnd){
    if(events.size()==1){
        return &*events.begin();
    }
    for (auto it = this->events.begin(); it != this->events.end(); it++){
        if(it->getTimeStart().hour() == timeStart.hour() && it->getTimeEnd().hour() == timeEnd.hour() &&
                it->getTimeStart().minute() == timeStart.minute() && it->getTimeEnd().minute() == timeEnd.minute()){
            return &*it;
        }
    }
    return nullptr;
}


QString DayElement::toString() {
    QString resStr = "\nDate: " + this->date.toString() + "\n";
    resStr += "List of events: \n";
    if(events.size()==1){
        return resStr+events.begin()->toString();
    }
    for(auto it = events.begin(); it != events.end(); ++it){
        resStr += it->toString();
    }
    return resStr;
}
