#ifndef DAYEVENT_H
#define DAYEVENT_H
#include <QTime>

using namespace std;
class DayEvent
{
private:
    QString text;
    QTime timeStart;
    QTime timeEnd;
    bool isDone;
    QString mail;
    int minutsBefore;
public:
    DayEvent(){};

    DayEvent(QString text, QTime timeStart, QTime timeEnd, bool isDone);

    DayEvent(QString text, QTime timeStart, QTime timeEnd, bool isDone, QString mail, int minutsBefore);

    ~DayEvent(){};

    QTime getTimeStart()const{return this->timeStart;}

    QTime getTimeEnd()const{return this->timeEnd;}

    QString getMail()const{return this->mail;}

    int getMinutesBefore()const{return this->minutsBefore;}

    bool getIsDone(){return this->isDone;}

    QString getText()const{return this->text;}

    void setIsDone(bool done){this->isDone = done;}

    QString toString();

    bool operator==(const DayEvent &other){
        return timeStart == other.timeStart && timeEnd == other.timeEnd;
    }

    bool operator!=(const DayEvent &other){
        return !(timeStart == other.timeStart && timeEnd == other.timeEnd);
    }

    void operator=(const DayEvent &other){
        this->mail = other.mail;
        this->text = other.text;
        this->timeEnd = other.timeEnd;
        this->timeStart = other.timeStart;
        this->isDone = other.isDone;
        this->minutsBefore = other.minutsBefore;
    }

    bool operator<(const DayEvent& other){
        return this->timeStart < other.timeStart;
    }
};

#endif // DAYEVENT_H
