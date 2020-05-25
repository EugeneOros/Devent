#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>




using namespace std;
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->dayTimer = new QTimer(this);
    this->dayTimer->setInterval(86400001-(QTime::currentTime().hour()*3600+QTime::currentTime().minute()*60+QTime::currentTime().second())*1000);
    connect(dayTimer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    dayTimer->start();
    this->setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/../../DoC/images/icon.png"));
    this->setWindowTitle("Devent");
    this->data = new Data(QCoreApplication::applicationDirPath() + "/../../DoC/data.json");
    if(data->contains(QDate::currentDate()))
         this->timer = new Timer(data->findDay(QDate::currentDate())->getEvents());
    else
        this->timer = new Timer();


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
    ui->calendarWidget->setSelectedDate(ui->calendarWidget->maximumDate());
    ui->calendarWidget->setSelectedDate(QDate::currentDate());

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
    if(ui->lineEdit_minuteStart->text()==""){
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
        try {
            data->addEvent(ui->calendarWidget->selectedDate(),DayEvent(text, QTime(hoursStart, minutesStart, 0), QTime(hoursEnd, minutesEnd, 0), false));

            if(ui->calendarWidget->selectedDate() == QDate::currentDate())
                this->timer->setEvents(data->findDay(QDate::currentDate())->getEvents());

            ui->lineEdit_text->setText("");
            ui->lineEdit_hourStart->setText("");
            ui->lineEdit_hourEnd->setText("");
            ui->lineEdit_minuteStart->setText("");
            ui->lineEdit_minuteEnd->setText("");
            QDate currentDate = ui->calendarWidget->selectedDate();
            ui->calendarWidget->setSelectedDate(ui->calendarWidget->maximumDate());
            ui->calendarWidget->setSelectedDate(currentDate);
        }catch (QString msg) {
            ui->label_info->setText(msg);
        }
    }else{
        ui->label_info->setStyleSheet("QLabel{text-align: center;font: 10pt \"Corbel\";color: #db0000;}");
        ui->label_info->setText("Warning. Fill all fields properly");
    }

}

void MainWindow::on_timeout()
{
    this->timer->setEvents(data->findDay(QDate::currentDate())->getEvents());
    this->dayTimer->stop();
    this->dayTimer->setInterval(86400001);
    this->dayTimer->start();
}

