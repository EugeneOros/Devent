#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QTime>
#include <dayevent.h>
#include <QMessageBox>
#include <QCoreApplication>
#include<QIcon>

using namespace std;
class Timer: public QObject
{
     Q_OBJECT
public:
    Timer(QObject *parent = nullptr);
    Timer(list<DayEvent>* events, QObject *parent = nullptr);
    virtual ~Timer() {}
    void startWorking();
    void setWorkingInterval();
    void stopWorking();
    void setMinutesList();
    void setEvents(list<DayEvent>* es);
private:
    void createWorkTimer();
    QTimer* WorkTimer;
    list<int> minutesList;
    list<DayEvent>* events;
    QMessageBox* messageBox;
    int currentMinutes;
    DayEvent* currentEvent;


//signals:

public slots:
    void on_timeout();
};



#endif // TIMER_H
