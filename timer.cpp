#include "timer.h"

Timer :: Timer(QObject* parent):QObject(parent){
    createWorkTimer();
}
Timer::Timer(list<DayEvent>* events, QObject *parent): QObject(parent)
{
    this->events = events;
    setMinutesList();
    createWorkTimer();
    setWorkingInterval();
    if(currentMinutes != -1){
        startWorking();
    }
}

void Timer::startWorking()
{
    WorkTimer->start();
}

void Timer::stopWorking()
{
    WorkTimer->stop();
}


void Timer::createWorkTimer()
{
    WorkTimer=new QTimer(this);
    WorkTimer->setInterval(3000);
    WorkTimer->setTimerType(Qt::VeryCoarseTimer);
    connect(WorkTimer,SIGNAL(timeout()),this,SLOT(on_timeout()));
}



void Timer::setEvents(list<DayEvent>* es){
    this->startWorking();
    this->events = es;
    this->setMinutesList();
    this->setWorkingInterval();
    this->startWorking();
}

void Timer::setMinutesList(){
     this->minutesList.clear();
    this->minutesList.clear();
    for(auto itEvent = this->events->begin(); itEvent != this->events->end(); ++itEvent){
        if (itEvent->getMail() != "")
            this->minutesList.push_back(itEvent->getTimeStart().hour()*60+itEvent->getTimeStart().minute() - itEvent->getMinutesBefore());
    }
    this->minutesList.sort();
}

void Timer::setWorkingInterval()
{

    currentMinutes = -1;
    for(auto itMinutes = this->minutesList.begin(); itMinutes != this->minutesList.end(); ++itMinutes){
        if(*itMinutes>QTime::currentTime().hour()*60+QTime::currentTime().minute()){
            WorkTimer->setInterval(*itMinutes*60*1000 - (QTime::currentTime().hour()*3600+QTime::currentTime().minute()*60+QTime::currentTime().second())*1000);
            this->minutesList.erase(itMinutes);
            currentMinutes = *itMinutes;
            break;
        }
    }
    if(currentMinutes == -1)
        stopWorking();
}

void Timer::on_timeout()
{
    for(auto itEvent = this->events->begin(); itEvent != this->events->end(); ++itEvent){
        if(itEvent->getTimeStart().hour()*60+itEvent->getTimeStart().minute()-itEvent->getMinutesBefore() == currentMinutes){
            currentEvent = &*itEvent;
        }
    }
    this->messageBox = new QMessageBox();
    this->messageBox->setText("Event heppening soon\n" + currentEvent->getText()+"\n"+currentEvent->getMail());
    messageBox->setWindowTitle("Devent");
    messageBox->setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/../../DoC/images/icon.png"));
    messageBox->setSizePolicy(QSizePolicy(QSizePolicy::Policy(500), QSizePolicy::Policy(1000)));
    messageBox->show();

    stopWorking();
    setWorkingInterval();
    if(currentMinutes != -1)
        startWorking();
}

