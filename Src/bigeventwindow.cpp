#include "bigeventwindow.h"
#include "ui_bigeventwindow.h"

BigEventWindow::BigEventWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::BigEventWindow){
    ui -> setupUi(this);
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("MaJCalc.db");
    if (!db.open()){
        QMessageBox::information(this, "错误", "数据库打开失败" + db.lastError().text());
    }
    connect(ui -> leadAddButton, &QPushButton:: clicked, this, &BigEventWindow:: addButtonClicked);
    connect(ui -> victimAddButton, &QPushButton:: clicked, this, &BigEventWindow:: addButtonClicked);
    connect(ui -> leadDelButton, &QPushButton:: clicked, this, &BigEventWindow:: delButtonClicked_Q);
    connect(ui -> victimDelButton, &QPushButton:: clicked, this, &BigEventWindow:: delButtonClicked_Q);
    connect(ui -> queryButton, &QPushButton:: clicked, this, &BigEventWindow:: queryButtonClicked);
    connect(ui -> submitButton, &QPushButton:: clicked, this, &BigEventWindow:: submitButtonClicked);
    connect(ui -> victimCombo_Q, &QComboBox:: activated, this, &BigEventWindow:: comboBoxModified);
    connect(ui -> leadCombo_Q, &QComboBox:: activated, this, &BigEventWindow:: comboBoxModified);
    connect(ui -> calendar, &QCalendarWidget:: clicked, this, &BigEventWindow:: calendarClicked);
    connect(ui -> historyDelButton, &QPushButton:: clicked, this, &BigEventWindow:: delButtonClicked);
    connect(ui -> clearButton, &QPushButton:: clicked, this, &BigEventWindow:: clearButtonClicked);
    //ui -> eventTable -> setColumnCount(4);
}

BigEventWindow::~BigEventWindow(){
    delete ui;
}

void BigEventWindow:: mainWindow_bigEventButtonClicked(){
    show();
    setWindowModality(Qt:: ApplicationModal);
    ui -> calendar -> setMaximumDate(QDate:: currentDate());
    ui -> leadCheck -> setChecked(false);
    ui -> victimCheck -> setChecked(false);
    ui -> dateCheck -> setChecked(false);
}

void BigEventWindow:: queryButtonClicked(){
    QString sql = "SELECT date, lead, victim, detail FROM bigEvent";
    bool oneChecked = false;
    if (ui -> dateCheck -> isChecked()){
        oneChecked = true;
        sql += " WHERE date LIKE '%1%'";
        if (ui -> calendar -> selectedDate().isNull()){
            QMessageBox:: information(this, "错误", "请选择日期！");
            return;
        }
        sql = sql.arg(ui -> calendar -> selectedDate().toString("yyyy年MM月dd日"));
    }
    if (ui -> leadCheck -> isChecked()){
        if (!oneChecked){
            oneChecked = true;
            sql += " WHERE ";
        }
        else
            sql += " AND ";
        sql += "lead LIKE '%%1%'";
        if (ui -> leadCombo_Q -> currentText().isEmpty()){
            QMessageBox:: information(this, "错误", "请选择主角！");
            return;
        }
        sql = sql.arg(ui -> leadCombo_Q -> currentText());
    }
    if (ui -> victimCheck -> isChecked()){
        if (!oneChecked){
            oneChecked = true;
            sql += " WHERE ";
        }
        else
            sql += " AND ";
        sql += "victim LIKE '%%1%'";
        if (ui -> victimCombo_Q -> currentText().isEmpty()){
            QMessageBox:: information(this, "错误", "请选择受害者！");
            return;
        }
    }
    /*
    if (!oneChecked){
        QMessageBox:: information(this, "错误", "你一个也不选，又让我查询，这让我很为难啊");
        return;
    }
    */
    sql += " ORDER BY date ASC";
    QSqlQueryModel *q = new QSqlQueryModel;
    q -> setQuery(sql);
    q -> setHeaderData(0, Qt:: Horizontal, "时间");
    q -> setHeaderData(1, Qt:: Horizontal, "主角");
    q -> setHeaderData(2, Qt:: Horizontal, "受害者");
    q -> setHeaderData(3, Qt:: Horizontal, "详情");

    qDebug() << q -> query().lastQuery();
    ui -> eventTable -> setModel(q);
    ui -> eventTable -> resizeColumnsToContents();
    ui -> eventTable -> verticalHeader() -> setHidden(true);
}

void BigEventWindow:: setComboName(const QStringList &list){
    QList<QComboBox*> children = findChildren<QComboBox*> ();
    for(QComboBox *combo: children){
        combo -> clear();
        combo -> addItems(list);
    }
}

void BigEventWindow:: addButtonClicked(){
    QListWidget *listWidget;
    QComboBox *comboBox;
    QPushButton *Sender = qobject_cast<QPushButton*>(sender());
    if (Sender == ui -> victimAddButton){
        listWidget = ui -> victimList;
        comboBox = ui -> victimCombo;
    }
    else{
        listWidget = ui -> leadList;
        comboBox = ui -> leadCombo;
    }
    listWidget -> addItem(comboBox -> currentText());
}

void BigEventWindow:: delButtonClicked_Q(){
    QListWidget *listWidget;
    QPushButton *Sender = qobject_cast<QPushButton*>(sender());
    if (Sender == ui -> victimDelButton){
        listWidget = ui -> victimList;
    }
    else{
        listWidget = ui -> leadList;
    }
    if (listWidget -> currentRow() >= 0){
        listWidget -> takeItem(listWidget -> currentRow());
    }
}

void BigEventWindow:: submitButtonClicked(){
    QSqlQuery q;
    QString sql = "INSERT INTO bigEvent VALUES('%1', '%2', '%3', '%4')";
    QString time = QDateTime:: currentDateTime().toString("yyyy年MM月dd日 hh:mm");
    QString lead, victim, detail;
    detail = ui -> detail -> toPlainText();
    detail.replace("\n", " ");
    for(int i = 0; i < ui -> victimList -> count(); i ++){
        if (i != 0)
            victim += ",";
        victim += ui -> victimList -> item(i) -> text();
    }
    for (int i = 0; i < ui -> leadList -> count(); i ++){
        if (i != 0)
            lead += ",";
        lead += ui -> leadList -> item(i) -> text();
    }
    sql = sql.arg(time, lead, victim, detail);
    if (!q.exec(sql)){
        QMessageBox:: warning(this, "错误", q.lastError().text() + q.lastQuery());
    }
}

void BigEventWindow:: comboBoxModified(){
    QComboBox *Sender = qobject_cast<QComboBox *>(sender());
    if (Sender == ui -> leadCombo_Q)
        ui -> leadCheck -> setChecked(true);
    else
        ui -> victimCheck -> setChecked(true);
}

void BigEventWindow:: calendarClicked(){
    ui -> dateCheck -> setChecked(true);
}

void BigEventWindow:: clearButtonClicked(){
    ui -> victimList -> clear();
    ui -> leadList -> clear();
    ui -> detail -> clear();
}

void BigEventWindow:: delButtonClicked(){
    int row = ui -> eventTable -> currentIndex().row();
    if (row == -1)
        return;
    QString date = ui -> eventTable -> model() -> index(row, 0).data().toString();
    QString lead = ui -> eventTable -> model() -> index(row, 1).data().toString();
    QString victim = ui -> eventTable -> model() -> index(row, 2).data().toString();
    QString detail = ui -> eventTable -> model() -> index(row, 3).data().toString();
    int ret = QMessageBox:: question(this, "警告", QString("是否要删除%1，主角为%2，受害者为%3，%4的记录？\n该操作不可恢复！").arg(date, lead, victim, detail));
    if (ret == QMessageBox:: Yes){
        QSqlQuery q;
        q.exec(QString("DELETE FROM bigEvent WHERE date = '%1' AND lead = '%2' AND victim = '%3' AND detail = '%4'").arg(date, lead, victim, detail));
        qDebug() << q.lastError() << q.lastQuery();
    }
    //queryButtonClicked();
    ui -> eventTable -> hideRow(row);
}
