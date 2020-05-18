#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum PageName{
    EVENT, CALENDAR, ADD, OPTIONS
};

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







class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void addEventObject(DayEvent event, DayElement element);

    void goToPage(PageName pageName);

    void goToEventDetails(DayEvent *event);

private slots:

    void on_calendarWidget_selectionChanged();

    void on_pushButton_add_clicked();

    void on_pushButton_calendar_clicked();

    void on_pushButton_options_clicked();

    void on_pushButton_addEvent_clicked();


private:
    Data *data;
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H


