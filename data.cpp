#include "data.h"


Data::Data(QString filePath)
{
    //open file
    this->filePath = filePath;
    QString strFile;
    QFile file;
    file.setFileName(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    strFile = file.readAll();
    file.close();
    //get array of days
    QJsonDocument jsonQDocumen = QJsonDocument::fromJson(strFile.toUtf8());
    this->jsonObject  = jsonQDocumen.object();
    QJsonValue daysQValue = jsonObject.value(QString("dayElement"));
    QJsonArray daysQArray = daysQValue.toArray();
    //write to days list
    for(auto itDay = daysQArray.begin(); itDay != daysQArray.end(); ++itDay){
        QJsonObject dayQObject = (*itDay).toObject();
        DayElement dayElement(QDate(dayQObject["year"].toInt(), dayQObject["month"].toInt(), dayQObject["day"].toInt()));

        QJsonArray eventsQArray = dayQObject["events"].toArray();
        for(auto itEvent=eventsQArray.begin(); itEvent!=eventsQArray.end(); ++itEvent){
            QJsonObject eventQObject = (*itEvent).toObject();
            QTime timeStart(eventQObject["hoursStart"].toInt(), eventQObject["minutesStart"].toInt(), 0);
            QTime timeEnd(eventQObject["hoursEnd"].toInt(), eventQObject["minutesEnd"].toInt(), 0);
            DayEvent event(eventQObject["text"].toString(), timeStart, timeEnd, eventQObject["isDone"].toBool(), eventQObject["mail"].toString(), eventQObject["minutesBefore"].toInt());
            dayElement.addEvent(event);
        }
        this->daysList.push_back(dayElement);
    }
}



QJsonObject Data::eventToJsonObject(DayEvent event){
    QJsonObject obj
    {
        {"hoursEnd", event.getTimeEnd().hour()},
        {"hoursStart", event.getTimeStart().hour()},
        {"mail", event.getMail()},
        {"minutesBefore", event.getMinutesBefore()},
        {"minutesEnd", event.getTimeEnd().minute()},
        {"minutesStart", event.getTimeStart().minute()},
        {"text", event.getText()},
        {"isDone", event.getIsDone()}
    };
    return obj;
}



QJsonObject Data::dayToJsonObject(DayElement element){
    QJsonArray eventsJsonArray = {};
    if(element.getEvents()->empty()){

    }else{
        list<DayEvent> *events = element.getEvents();
        for(auto itEvent=events->begin(); itEvent!=events->end(); ++itEvent ){
            eventsJsonArray.append(eventToJsonObject(*itEvent));
        }
    }
    QJsonObject obj
    {
        {"day", element.getDay()},
        {"month", element.getMonth()},
        {"year", element.getYear()},
        {"events", eventsJsonArray}
    };
    return obj;
}



void Data::addDay(DayElement element){
    if(this->contains(element.getDate()))
        return;
    this->daysList.push_back(element);
    QJsonArray daysQArray = this->jsonObject.value(QString("dayElement")).toArray();
    daysQArray.append(dayToJsonObject(element));
    this->jsonObject["dayElement"] = daysQArray;
    this->write();
}



void Data::addEvent(QDate date, DayEvent event){
    if(!this->contains(date))
        addDay(DayElement(date));
    list<DayElement> dayList = this->daysList;
    DayElement* dayPtr = nullptr;
    for(auto itDay=this->daysList.begin(); itDay!=this->daysList.end(); ++itDay){
        if(itDay->getDate() == date){
            dayPtr = &*itDay;
            break;
        }
    }
    if(dayPtr==nullptr){
        throw "Cant add event pointer to day";
    }
    dayPtr->addEvent(event);
    QJsonArray daysJsonArray = this->jsonObject.value(QString("dayElement")).toArray();
    QJsonArray eventJsonArray = {};
    QJsonObject dayJsonObject;
    for(auto itJsonDay=daysJsonArray.begin(); itJsonDay!=daysJsonArray.end(); ++itJsonDay){
        if(itJsonDay->toObject()["day"]==date.day() && itJsonDay->toObject()["month"]==date.month() && itJsonDay->toObject()["year"]==date.year()){
            eventJsonArray = itJsonDay->toObject().value(QString("events")).toArray();
            eventJsonArray.append(eventToJsonObject(event));
            dayJsonObject = itJsonDay->toObject();
            dayJsonObject["events"] = eventJsonArray;
            daysJsonArray.erase(itJsonDay);
            break;
        }
    }

    daysJsonArray.append(dayJsonObject);
    this->jsonObject["dayElement"] = daysJsonArray;

    this->write();
}



void Data::removeDay(QDate date){
    DayElement* toRemove = findDay(date);
    if(toRemove==nullptr)
        return;
    this->daysList.remove(*toRemove);

    QJsonArray daysJsonArray = this->jsonObject.value(QString("dayElement")).toArray();
    for(auto itJsonDay=daysJsonArray.begin(); itJsonDay!=daysJsonArray.end(); ++itJsonDay){
        if(itJsonDay->toObject()["day"]==date.day() && itJsonDay->toObject()["month"]==date.month() && itJsonDay->toObject()["year"]==date.year()){
            daysJsonArray.erase(itJsonDay);
            break;
        }
    }
    this->jsonObject["dayElement"] = daysJsonArray;
    this->write();
}



void Data::setIsDone(DayElement *dayElement, DayEvent *event, bool isDone){
   if(dayElement == nullptr){
       return;
   }
   if(event == nullptr){
       return;
   }
   event->setIsDone(isDone);
}



void Data::removeEvent(QDate date, QTime timeStart, QTime timeEnd){
   DayElement* dayEl = findDay(date);
   if(dayEl == nullptr){
       return;
   }
   DayEvent* event = dayEl->findEvent(timeStart, timeEnd);

   if(event == nullptr){
       return;
   }
   DayEvent e = *event;
   dayEl->removeEvent(e);

   QJsonArray daysJsonArray = this->jsonObject.value(QString("dayElement")).toArray();
   QJsonArray eventJsonArray = {};
   QJsonObject dayJsonObject;
   for(auto itJsonDay=daysJsonArray.begin(); itJsonDay!=daysJsonArray.end(); ++itJsonDay){

       if(itJsonDay->toObject()["day"].toInt()==date.day() && itJsonDay->toObject()["month"].toInt()==date.month() && itJsonDay->toObject()["year"].toInt()==date.year()){

           eventJsonArray = itJsonDay->toObject().value(QString("events")).toArray();
           for(auto itJsonEvent=eventJsonArray.begin(); itJsonEvent!=eventJsonArray.end(); ++itJsonEvent)
           {

               if(itJsonEvent->toObject()["hoursStart"].toInt()==event->getTimeStart().hour() &&
                       itJsonEvent->toObject()["minutesStart"].toInt()==event->getTimeStart().minute() &&
                       itJsonEvent->toObject()["hoursEnd"].toInt()==event->getTimeEnd().hour() &&
                       itJsonEvent->toObject()["minutesEnd"].toInt()==event->getTimeEnd().minute()){
                   eventJsonArray.erase(itJsonEvent);
                   break;
               }

           }

           dayJsonObject = itJsonDay->toObject();
           dayJsonObject["events"] = eventJsonArray;
           daysJsonArray.erase(itJsonDay);
           break;
       }
   }

   daysJsonArray.append(dayJsonObject);
   this->jsonObject["dayElement"] = daysJsonArray;

   this->write();

}



bool Data::contains(QDate date){
    return findDay(date) != NULL;
}



DayElement* Data::findDay(QDate date){
//    if(this->daysList.size()==1){
//        return &*this->daysList.begin();
//    }
    for(auto itDay=this->daysList.begin(); itDay!=this->daysList.end(); ++itDay){
        if(itDay->getDate() == date){
            return &*itDay;
        }
    }
    return nullptr;
}








Data::~Data(){

}



void Data::write(){
    QFile file;
    file.setFileName(filePath);
    file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QJsonDocument saveDoc(this->jsonObject);
    file.write(saveDoc.toJson());
    file.close();

}












//list<DayElement> Data::getDaysList(){
//    return this->daysList;
//      //open file
//      QString strFile;
//      QFile file;
//      file.setFileName("C:/Users/Eugen/Downloads/data.json");
//      file.open(QIODevice::ReadOnly | QIODevice::Text);
//      strFile = file.readAll();
//      file.close();
//      //get array of days
//      jsonQDocumen = QJsonDocument::fromJson(strFile.toUtf8());
//      QJsonObject mainQObject = doc.object();
//      QJsonValue daysQValue = mainQObject.value(QString("dayElement"));
//      QJsonArray daysQArray = daysQValue.toArray();
//      //write to days list
//      list<DayElement> daysList;
//      for(int i=0; i<daysQArray.size(); i++){
//          QJsonObject dayQObject = daysQArray.takeAt(i).toObject();
//          DayElement dayElement(dayQObject["day"].toInt(), dayQObject["month"].toInt(), dayQObject["year"].toInt());

//          QJsonArray eventsQArray = dayQObject["dayEvents"].toArray();
//          for(int j=0; j<eventsQArray.size(); j++){
//              QJsonObject eventQObject = eventsQArray.takeAt(j).toObject();
//              QTime timeStart(eventQObject["hoursStart"].toInt(), eventQObject["minutesStart"].toInt(), 0);
//              QTime timeEnd(eventQObject["hoursEnd"].toInt(), eventQObject["minutesEnd"].toInt(), 0);
//              DayEvent event(eventQObject["text"].toString(), timeStart, timeEnd, false, eventQObject["mail"].toString(), eventQObject["minutesBefore"].toInt());
//              dayElement.addEvent(event);
//          }
//          daysList.push_back(dayElement);
//      }
//      return daysList;
//}



//      QJsonDocument doc(value.toArray().takeAt(0).toObject());
//      int a = value.toArray().takeAt(0).toObject()["day"].toInt();
//      QString s = QString::number(a);

//      value.toArray().takeAt(0).toObject()["day"];
//      QString strJson(doc.toJson(QJsonDocument::Indented));


//      list<DayElement> days();
//      qWarning() << val;
//      QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
//      QJsonObject sett2 = d.object();
//      QJsonValue value = sett2.value(QString("appName"));
//      qWarning() << value;
//      QJsonObject item = value.toObject();
//      qWarning() << tr("QJsonObject of description: ") << item;

//      /* in case of string value get value and convert into string*/
//      qWarning() << tr("QJsonObject[appName] of description: ") << item["description"];
//      QJsonValue subobj = item["description"];
//      qWarning() << subobj.toString();

//      /* in case of array get array and convert into string*/
//      qWarning() << tr("QJsonObject[appName] of value: ") << item["imp"];
//      QJsonArray test = item["imp"].toArray();
//      qWarning() << test[1].toString();
