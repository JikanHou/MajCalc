#include "gamehistorywindow.h"
#include "ui_gamehistorywindow.h"

GameHistoryWindow:: GameHistoryWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::GameHistoryWindow){
    ui -> setupUi(this);
    ui -> table -> horizontalHeader() -> setStretchLastSection(true);
    ui -> table -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
    connect(ui -> calendar, &QCalendarWidget:: selectionChanged, this, &GameHistoryWindow:: calenderModified);
}

GameHistoryWindow:: ~GameHistoryWindow(){
    delete ui;
}

void GameHistoryWindow:: mainWindow_CalendarButtonClicked(){
    setWindowModality(Qt:: ApplicationModal);
    show();
    ui -> calendar -> setMaximumDate(QDate:: currentDate());
    ui -> calendar -> setSelectedDate(QDate:: currentDate());
}

void GameHistoryWindow:: calenderModified(){
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("MaJCalc.db");
    if (!db.open()){
        QMessageBox::information(this, "错误", "数据库打开失败");
    }
    QSqlQuery q;
    q.exec(QString("SELECT player, rank, point, date FROM gameHistory WHERE date LIKE '%1%' ORDER BY date ASC").arg(ui -> calendar -> selectedDate().toString("yyyy年MM月dd日")));
    qDebug() << q.lastQuery();
    ui -> table -> clearContents();
    ui -> table -> setRowCount(0);
    QString lastDate;
    int row = 0;
    while (q.next()){
        if (q.value(3).toString() != lastDate){
            ui -> table -> insertRow(row ++);
            lastDate = q.value(3).toString();
            ui -> table -> setItem(row - 1, 0, new QTableWidgetItem(lastDate));
            ui -> table -> item(row - 1, 0) -> setTextAlignment(Qt::AlignHCenter | Qt:: AlignVCenter);
        }
        QString player = q.value(0).toString();
        int rank = q.value(1).toInt(), point = q.value(2).toInt();
        ui -> table -> setItem(row - 1, rank, new QTableWidgetItem(QString("%1 %2").arg(player).arg(point)));
        ui -> table -> item(row - 1, rank) -> setTextAlignment(Qt::AlignHCenter | Qt:: AlignVCenter);
    }
}
