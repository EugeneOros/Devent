#ifndef DATA_H
#define DATA_H
#include "dayelement.h"
#include <QString>
#include <QIODevice>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include "dayelement.h"

class Data
{
private:
    QJsonObject jsonObject;

    list<DayElement> daysList;

    QString filePath;

    QString tmp;

public:
    Data(QString filePath);

    ~Data();

    list<DayElement> getDaysList(){return this->daysList;};

    QString getFilePath(){return this->filePath;}

    QJsonObject getJsonObject(){return this->jsonObject;}

    QString getTmp(){return this->tmp;}

    void setIsDone(DayElement *dayElement, DayEvent *event, bool isDone);

    void write();

    void addDay(DayElement element);

    void addEvent(QDate date, DayEvent event);

    void removeEvent(QDate date, QTime timeStart, QTime timeEnd);

    void removeDay(QDate date);

    DayElement* findDay(QDate date);

    QJsonObject dayToJsonObject(DayElement element);

    QJsonObject eventToJsonObject(DayEvent event);

    bool contains(QDate date);

};

#endif // DATA_H
