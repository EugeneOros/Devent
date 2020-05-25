#include "frameevent.h"
#include "ui_mainwindow.h"
#include <QDir>


FrameEvent::FrameEvent( DayElement *day, DayEvent *event, Data *data, MainWindow *parent) : QFrame(parent), parent(parent), data(data){
    this->event = event;
    this->day = day;

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(11, 0, 11, 0);
    this->setLayout(layout);
    if(event->getIsDone()){
        this->setStyleSheet("QFrame{border-bottom: 1px solid #6df; border-radius:20px;background-color: #0f4;}"
                            "QFrame:hover{background-color: #6df;}");
    }else{
        this->setStyleSheet("QFrame{border-bottom: 1px solid #6df; border-radius:20px;}"
                            "QFrame:hover{background-color: #6df;}");
    }


    checkBox = new QCheckBox(this);
    checkBox->setTristate(event->getIsDone());
    checkBox->setFixedSize(QSize(15,15));
    checkBox->setStyleSheet("QCheckBox{border:none;background-color:  transparent;}");
    checkBox->setTristate(false);
    this->checkBox->setChecked(event->getIsDone());
    connect(checkBox, SIGNAL (toggled(bool)), this, SLOT (onDone()));

    layout->addWidget(checkBox);

    pushButton_text = new QPushButton(event->getText(),this);
    pushButton_text->setStyleSheet("QPushButton {font: 13pt \"Corbel\"; color: #f4fafb; background-color:  transparent;}"
                              "QPushButton:hover{background-color:  transparent; color: #f4fafb;}"
                              "QPushButton:pressed{background-color:  transparent;color: #43454f;}");
    pushButton_text->setFixedHeight(60);
    connect(pushButton_text, SIGNAL (clicked()), this, SLOT (onClick()));
    layout->addWidget(pushButton_text);


    QPushButton *pushButton_delete = new QPushButton(this);
    pushButton_delete->setFixedSize(QSize(40,40));
    QIcon icon;
    icon.addFile(QCoreApplication::applicationDirPath() + "/../../DoC/images/delete.png");
    pushButton_delete->setIcon(icon);
    pushButton_delete->setIconSize(QSize(40, 40));
    pushButton_delete->setStyleSheet("""QPushButton{border:none;background-color:  transparent;}""");
    connect(pushButton_delete, SIGNAL (clicked()), this, SLOT (onDelete()));
//    pushButton_delete->setCursor(QCursor(Qt.PointingHandCursor));
    layout->addWidget(pushButton_delete);

}

void FrameEvent::onDelete(){
    data->removeEvent(day->getDate(), event->getTimeStart(), event->getTimeEnd());
    this->pushButton_text->setText(data->getTmp());
    this->deleteLater();

}

void FrameEvent::onClick(){
    if(parent != nullptr){
        this->parent->goToEventDetails(event);
    }
}



void FrameEvent::onDone(){
    data->setIsDone(day, event, !(event->getIsDone()));
    if(this->checkBox->isChecked()){
        this->setStyleSheet("QFrame{border-bottom: 1px solid #6df; border-radius:20px;background-color: #0f4;}"
                                "QFrame:hover{background-color: #6df;}");
    }else{
        this->setStyleSheet("QFrame{border-bottom: 1px solid #6df; border-radius:20px;}"
                                "QFrame:hover{background-color: #6df;}");
    }
}

