#ifndef FRAMEEVENT_H
#define FRAMEEVENT_H

#include <QMainWindow>
#include <QFrame>
#include "dayevent.h"
#include "dayelement.h"
#include "data.h"
#include <QString>
#include <QIODevice>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include<list>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QLayout>
#include <QGroupBox>
#include <QSpacerItem>
#include <Qt>
#include <QCheckBox>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QSizePolicy>
#include "mainwindow.h"

class MainWindow;

class FrameEvent : public QFrame
{
    Q_OBJECT

public:
    FrameEvent( DayElement* day, DayEvent* event, Data* data,  MainWindow *parent = nullptr );

    ~FrameEvent(){};

private slots:
    void onDelete();

    void onClick();

    void onDone();

private:
    MainWindow *parent;

    DayElement *day;

    DayEvent *event;

    Data *data;

    QPushButton *pushButton_text;

    QCheckBox *checkBox;
};

#endif // FRAMEEVENT_H
