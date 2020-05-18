#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>




using namespace std;
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/../../DoC/images/icon.png"));
    this->setWindowTitle("Devent");
    this->data = new Data(QCoreApplication::applicationDirPath() + "/../../DoC/data.json");



    //start state
    goToPage(CALENDAR);
    ui->label_date->setText(QDate::currentDate().toString());
    this->setStyleSheet("QScrollBar:vertical{background-color: #2A2929;width: 17px; margin: 15px 3px 15px 3px; border: 1px transparent #2A2929;border-radius: 5px;}"

                        "QScrollBar::handle:vertical{background-color: #43454f; min-height: 5px;border-radius: 5px;}"

                        "QScrollBar::sub-line:vertical{margin: 3px 0px 3px 0px;border-image: url(:/qss_icons/rc/up_arrow_disabled.png);"
                            "height: 10px;width: 10px;subcontrol-position: top;subcontrol-origin: margin;}"

                        "QScrollBar::add-line:vertical{margin: 3px 0px 3px 0px;border-image: url(:/qss_icons/rc/down_arrow_disabled.png);"
                            "height: 10px;width: 10px;subcontrol-position: bottom;subcontrol-origin: margin;}"

                        "QScrollBar::sub-line:vertical:hover,QScrollBar::sub-line:vertical:on{border-image: url(:/qss_icons/rc/up_arrow.png);"
                            "height: 10px;width: 10px;subcontrol-position: top;subcontrol-origin: margin;}"


                        "QScrollBar::add-line:vertical:hover, QScrollBar::add-line:vertical:on{border-image: url(:/qss_icons/rc/down_arrow.png);"
                            "height: 10px;width: 10px;subcontrol-position: bottom;subcontrol-origin: margin;}"

                        "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical{background: none;}"


                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{background: none;}");
    QPixmap pixmap(QCoreApplication::applicationDirPath() + "/../../DoC/images/icon.png");
    ui->label_picon->setPixmap(pixmap);

}

MainWindow::~MainWindow()
{
    delete ui;

}

//void MainWindow::addEventObject(DayEvent event, DayElement element){
//    QFrame frame_event(ui->scrollArea);
//}





void MainWindow::on_calendarWidget_selectionChanged()
{
    ui->label_date->setText(ui->calendarWidget->selectedDate().toString());
    DayElement* day = data->findDay(ui->calendarWidget->selectedDate());

    QLayout *layout = ui->scrollArea->widget()->layout();
    QSpacerItem *spacer = ui->spacer_scroll;
    layout->removeItem(spacer);
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != 0) {
        if (child->widget() != NULL)
            {
                delete (child->widget());
            }
        delete child;
    }
    if(day != nullptr){
        for(auto itEvent=day->getEvents()->begin(); itEvent!=day->getEvents()->end(); ++itEvent){
            QFrame *newFrame = new FrameEvent(day, &*itEvent, data, this);
//            connect(newFrame, SIGNAL (clicked()), this, SLOT (goToEventDetails(&*itEvent)));
            layout->addWidget(newFrame);
        }
    }
    layout->addItem(spacer);
}

void MainWindow::on_pushButton_add_clicked()
{
    goToPage(ADD);

}

void MainWindow::goToEventDetails(DayEvent *event){
    this->goToPage(EVENT);
    ui->label_textDResult->setText(event->getText());
    ui->label_startTimeDResult->setText(event->getTimeStart().toString());
    ui->label_endTimeDResult->setText(event->getTimeEnd().toString());
    ui->label_minutesDResult->setText(QString::number(event->getMinutesBefore()));
    ui->label_emailDResult->setText(event->getMail());
}


void MainWindow::goToPage(PageName pageName){
    switch (pageName) {
    case CALENDAR:
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case EVENT:
        ui->stackedWidget->setCurrentIndex(1);
        break;
    case ADD:
        ui->stackedWidget->setCurrentIndex(2);
        break;
    case OPTIONS:
        ui->stackedWidget->setCurrentIndex(3);
        break;
    }


}

void MainWindow::on_pushButton_calendar_clicked()
{
    goToPage(CALENDAR);
}

void MainWindow::on_pushButton_options_clicked()
{
    goToPage(OPTIONS);
}

void MainWindow::on_pushButton_addEvent_clicked()
{
    bool isCorrect = true;
    if(ui->lineEdit_text->text() == ""){
        isCorrect = false;

    }
    if(ui->lineEdit_hourStart->text()==""){
        isCorrect = false;
    }
    if(ui->lineEdit_hourEnd->text()==""){
        isCorrect = false;
    }
    if(ui->lineEdit_minuteEnd->text()==""){
        isCorrect = false;
    }
    if(ui->lineEdit_minuteEnd->text()==""){
        isCorrect = false;
    }
    if(isCorrect){
        QString text = ui->lineEdit_text->text();
        int hoursStart = ui->lineEdit_hourStart->text().toInt();
        int hoursEnd = ui->lineEdit_hourEnd->text().toUInt();
        int minutesStart = ui->lineEdit_minuteStart->text().toUInt();
        int minutesEnd = ui->lineEdit_minuteEnd->text().toUInt();
        ui->label_info->setStyleSheet("QLabel{text-align: center;font: 10pt \"Corbel\";color: #83a836;}");
        ui->label_info->setText("Info. The event was seccesfuly added");
        data->addEvent(ui->calendarWidget->selectedDate(),DayEvent(text, QTime(hoursStart, minutesStart, 0), QTime(hoursEnd, minutesEnd, 0), false));
    }else{
        ui->label_info->setStyleSheet("QLabel{text-align: center;font: 10pt \"Corbel\";color: #db0000;}");
        ui->label_info->setText("Warning. Fill all fields properly");
    }

}

/////////////////////////////////////////////////////////////////////

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
//    this->setStyleSheet("QFrame{border-bottom: 1px solid #6df; border-radius:20px;background-color: #0f4;}"
//                        "QFrame:hover{background-color: #6df;}");
    data->setIsDone(day, event, !(event->getIsDone()));
    if(this->checkBox->isChecked()){
        this->setStyleSheet("QFrame{border-bottom: 1px solid #6df; border-radius:20px;background-color: #0f4;}"
                                "QFrame:hover{background-color: #6df;}");
    }else{
        this->setStyleSheet("QFrame{border-bottom: 1px solid #6df; border-radius:20px;}"
                                "QFrame:hover{background-color: #6df;}");
    }
}
