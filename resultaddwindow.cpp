#include "resultaddwindow.h"
#include "ui_resultaddwindow.h"

ResultAddWindow::ResultAddWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::ResultAddWindow){
    ui->setupUi(this);
    for(int i = 0; i < 4; i ++){
        QString comboName = "comboBox_*", editName = "lineEdit_*";
        comboName[comboName.length() - 1] = char('1' + i);
        editName[editName.length() - 1] = char('1' + i);
        playerName[i] = findChild<QComboBox*>(comboName);
        playerPoint[i] = findChild<QLineEdit*>(editName);
    }
    connect(ui -> confirm, &QAction:: triggered, this, &ResultAddWindow:: resultAddWindow_ConfirmButtonClicked);
    connect(ui -> cancel, &QAction:: triggered, this, &ResultAddWindow:: resultAddWindow_CancelButtonClicked);
    for(int i = 0; i < 4; i ++)
        connect(playerPoint[i], &QLineEdit:: textChanged, this, &ResultAddWindow:: pointsModified);

}

ResultAddWindow::~ResultAddWindow(){
    delete ui;
}

void ResultAddWindow:: resultAddWindow_CancelButtonClicked(){
    hide();
    setWindowModality(Qt::NonModal);
}

void ResultAddWindow:: resultAddWindow_ConfirmButtonClicked(){
    HanCyan res;
    for(int i = 0; i < 4; i ++){
        bool ok;
        QString name = playerName[i] -> currentText();
        int point = playerPoint[i] -> text().toInt(&ok);
        if (!ok){
            QMessageBox:: information(this, "错误", "玩家 " + name +" 的分数不合法！");
            return;
        }
        res.addPlayer(name, point);
    }
    if (ui -> sum -> text() != "总和：100000"){
        int ret = QMessageBox:: question(this, "警告", "玩家总分不为100000，是否确认上传？");
        if (ret != QMessageBox:: Ok){
            return;
        }
    }
    if (res.size() != 4){
        QMessageBox:: information(this, "错误", "玩家姓名重复，请检查！");
        return;
    }
    hide();
    setWindowModality(Qt:: NonModal);
    if (mode == addWindowMode:: add){
        res.setGameTime();
        emit resultAddWindow_ConfirmButtonClickedSignal(res);
    }
    else{
        res.setGameTime(QDateTime:: fromString(currentTime, "yyyy年M月d日 hh:mm"));
        emit resultAddWindow_ConfirmButtonClickedSignal_Modify(res);
    }
}

void ResultAddWindow:: mainWindow_ResultAddButtonClicked(){
    setWindowModality(Qt:: ApplicationModal);
    show();
    mode = addWindowMode:: add;
    for(int i = 0; i < 4; i ++)
        playerPoint[i] -> setText("");
}

void ResultAddWindow:: modifyPlayerName(QStringList list){
    list.removeAll("\n");
    list.removeAll("");
    for (int i = 0; i < 4; i ++){
        playerName[i] -> clear();
        playerName[i] -> addItems(list);
    }
}

void ResultAddWindow:: pointsModified(){
    int sum = 0;
    for(int i = 0; i < 4; i ++){
        bool ok; int p;
        p = playerPoint[i] -> text().toInt(&ok);
        if (ok)
            sum += p;
    }
    ui -> sum -> setText(QString("总和：%1").arg(sum));
}

void ResultAddWindow:: mainWindow_ResultModifyButtonClicked(QString date){
    mode = addWindowMode:: modify;
    currentTime = date;
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
    q.exec(QString("SELECT player, rank, point FROM gameHistory WHERE date = '%1'").arg(date));
    qDebug() << q.lastError();
    int sum = 0;
    while (q.next()){
        QString player = q.value(0).toString();
        int rank = q.value(1).toInt(), point = q.value(2).toInt();
        playerName[rank - 1] -> setCurrentText(player);
        playerPoint[rank - 1] -> setText(QString:: number(point));
        sum += point;
    }
    ui -> sum -> setText(QString("总和：%1").arg(sum));
    show();
    setWindowModality(Qt:: ApplicationModal);
}
