#include "dayevent.h"

DayEvent:: DayEvent(QString text, QTime timeStart, QTime timeEnd,  bool isDone):
    text(text), timeStart(timeStart), timeEnd(timeEnd), isDone(isDone){
    this->minutsBefore = 0;
    this->mail = "";
}

DayEvent:: DayEvent(QString text, QTime timeStart, QTime timeEnd, bool isDone, QString mail, int minutsBefore):
text(text), timeStart(timeStart), timeEnd(timeEnd), isDone(isDone), mail(mail), minutsBefore(minutsBefore){

}

QString DayEvent::toString() {
    QString resStr = "\ntext: " + this->text + "\n";
    resStr += "start time: " + this->getTimeStart().toString() + "\n";
    resStr += "end time: " + this->getTimeEnd().toString() + "\n";
    if(this->isDone){
        resStr += "isDone: Yes\n";
    }else if(!-this->isDone){
        resStr += "isDone: No\n";
    }
    resStr += "mail: " + this->mail + "\n";
    QString minutesBefore = QString::number(this->minutsBefore);
    resStr += "time to send mail: " + minutesBefore + "m befor start event\n\n";
    return resStr;
}
