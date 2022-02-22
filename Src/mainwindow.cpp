#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow:: MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui -> setupUi(this);
    resultAddWindow = new ResultAddWindow();
    settingsWindow = new SettingsWindow();
    statisticWindow = new StatisticWindow();
    gameHistoryWindow = new GameHistoryWindow();
    bigEventWindow = new BigEventWindow();

    connect(ui -> resultUpload, &QAction:: triggered, resultAddWindow, &ResultAddWindow:: mainWindow_ResultAddButtonClicked);
    connect(resultAddWindow, &ResultAddWindow:: resultAddWindow_ConfirmButtonClickedSignal, this, &MainWindow:: resultAddWindow_ConfirmButtonClicked);

    connect(ui -> settings, &QAction:: triggered, settingsWindow, &SettingsWindow:: mainWindow_SettingsWindowClicked);
    connect(settingsWindow, &SettingsWindow:: settingsWindow_ConfirmButtonClickedSignal, this, &MainWindow:: settingsWindow_ConfirmButtonClicked);

    connect(ui -> pointTable, &QTableWidget:: cellDoubleClicked, this, &MainWindow:: mainWindow_modifyButtonClicked);
    connect(ui -> resultModify, &QAction:: triggered, this, &MainWindow:: mainWindow_modifyButtonClicked);
    connect(this, &MainWindow:: mainWindow_ResultModifyButtonClickedSignal, resultAddWindow, &ResultAddWindow:: mainWindow_ResultModifyButtonClicked);
    connect(resultAddWindow, &ResultAddWindow:: resultAddWindow_ConfirmButtonClickedSignal_Modify, this, &MainWindow:: resultAddWindow_ConfirmButtonClicked_Modify);

    connect(ui -> resultDelete, &QAction:: triggered, this, &MainWindow:: mainWindow_DeleteButtonClicked);

    connect(ui -> resultStatistic, &QAction:: triggered, statisticWindow, &StatisticWindow:: mainWindow_StatisticButtonClicked);
    connect(ui -> gameHistory, &QAction:: triggered, gameHistoryWindow, &GameHistoryWindow:: mainWindow_CalendarButtonClicked);
    connect(ui -> bigEvent, &QAction:: triggered, bigEventWindow, &BigEventWindow:: mainWindow_bigEventButtonClicked);

    databaseSetup();
    restoreSettings();
    resultAddWindow -> modifyPlayerName(settingsWindow -> getPlayerList());
    bigEventWindow -> setComboName(settingsWindow -> getPlayerList());
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow:: restoreSettings(){
    QSettings settings("HouJikan", "MJCalc");
    QDateTime lastTime;
    lastTime = settings.value("lastTime", QDateTime:: currentDateTime()).toDateTime();
    QDateTime now = QDateTime:: currentDateTime();
    QStringList _headerList, moneyHeaderList;
    ui -> pointTable -> clearContents();
    _headerList << "对局时间" << "一位" << "二位" << "三位" << "四位";
    ui -> pointTable -> setColumnCount(5);
    ui -> pointTable -> setRowCount(0);
    ui -> pointTable -> setHorizontalHeaderLabels(_headerList);


    ui -> moneyTable -> clearContents();
    moneyHeaderList << "姓名" << "总计";
    ui -> moneyTable -> setColumnCount(2);
    ui -> moneyTable -> setHorizontalHeaderLabels(moneyHeaderList);

    if (lastTime.secsTo(now) >= 8 * 60 * 60){//8 hours later
        settings.setValue("gameList", QStringList());
    }
    else{
        QStringList gameList = settings.value("gameList").toStringList();
        int row = 0;
        for (QString date: gameList){
            addRecordFromDB(date, row);
        }
        updateMoney();
    }
    ui -> pointTable -> horizontalHeader() -> setStretchLastSection(true);
    ui -> pointTable -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
    ui -> moneyTable -> horizontalHeader() -> setStretchLastSection(true);
    ui -> moneyTable -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow:: addRecordFromDB(const QString &date, int &row){
    QSqlQuery query;
    QString sql = "SELECT player, rank, point FROM gameHistory where date='%1'";
    sql = sql.arg(date);
    if (!query.exec(sql)){
        qDebug() << "addRecordFromDB" << query.lastError();
        return;
    }
    ui -> pointTable -> insertRow(row);
    ui -> pointTable -> setItem(row, 0, new QTableWidgetItem(date));
    ui -> pointTable -> item(row, 0) -> setTextAlignment(Qt::AlignHCenter | Qt:: AlignVCenter);
    bool ok = false;

    while (query.next()){
        int rank = query.value(1).toInt();
        QString point = query.value(2).toString();
        QString player = query.value(0).toString();
        ui -> pointTable -> setItem(row, rank, new QTableWidgetItem(player + " " + point));
        ui -> pointTable -> item(row, rank) -> setTextAlignment(Qt::AlignHCenter | Qt:: AlignVCenter);
        ok = true;
    }
    if (ok)
        row += 1;
    else
        ui -> pointTable -> removeRow(row);
}

void MainWindow:: updateMoney(){
    QMap<QString, double> map;
    ui -> moneyTable -> clearContents();
    ui -> moneyTable -> setRowCount(0);
    QStringList umas = settingsWindow -> getUma();
    int rate = settingsWindow -> getRate(), kaeshi = settingsWindow -> getKaeshi();
    QStringList list;
    for(int i = 0; i < ui -> pointTable -> rowCount(); i ++){
        for (int j = 1; j <= 4; j ++){
            QString item = ui -> pointTable -> item(i, j) -> text();
            QStringList res = item.split(' ');
            QString name = res[0];
            double point = res[1].toDouble();
            point = (point - kaeshi) / rate + umas[j - 1].toDouble();
            map[name] += point;
        }
        list << ui -> pointTable -> item(i, 0) -> text();
    }
    QSettings settings("HouJikan", "MJCalc");
    settings.setValue("gameList", list);
    int row = 0;
    for (QMap<QString, double>:: iterator it = map.begin(); it != map.end(); ++ it){
        ui -> moneyTable -> insertRow(row);
        ui -> moneyTable -> setItem(row, 0, new QTableWidgetItem(it.key()));
        ui -> moneyTable -> setItem(row, 1, new QTableWidgetItem(QString:: number(it.value(), 'f', 1)));
        ui -> moneyTable -> item(row, 0) -> setTextAlignment(Qt::AlignHCenter | Qt:: AlignVCenter);
        ui -> moneyTable -> item(row, 1) -> setTextAlignment(Qt::AlignHCenter | Qt:: AlignVCenter);
        qDebug() << ui -> moneyTable -> item(row, 1) -> text();
        row ++;
    }
    ui -> gameCount -> setText(QString("对战局数：%1").arg(ui -> pointTable -> rowCount()));
}

void MainWindow:: resultAddWindow_ConfirmButtonClicked(HanCyan result){
    QStringList rank = result.getRank();
    if (rank.isEmpty()){
        QMessageBox:: information(this, "错误", "玩家信息有误，请重新检查！");
        return;
    }
    int row = ui -> pointTable -> rowCount();
    ui -> pointTable -> insertRow(row);
    for(int i = 0; i < 4; i ++){
        ui -> pointTable -> setItem(row, i + 1, new QTableWidgetItem(rank[i] + " " + QString:: number(result.getPoint(rank[i]))));
        ui -> pointTable -> item(row, i + 1) -> setTextAlignment(Qt::AlignHCenter | Qt:: AlignVCenter);
    }
    QSettings settings("HouJikan", "MJCalc");
    QDateTime currentDate_t = result.getGameTime();
    while (currentDate_t.secsTo(settings.value("lastTime", QDateTime:: currentDateTime()).toDateTime()) > -60)
        currentDate_t = currentDate_t.addSecs(60);
    settings.setValue("lastTime", currentDate_t);
    QString currentDate = currentDate_t.toString("yyyy年MM月dd日 hh:mm");
    ui -> pointTable -> setItem(row, 0, new QTableWidgetItem(currentDate));
    ui -> pointTable -> item(row, 0) -> setTextAlignment(Qt::AlignHCenter | Qt:: AlignVCenter);
    updateMoney();
    ui -> gameCount -> setText(QString("对战局数：%1").arg(ui -> pointTable -> rowCount()));

    //数据库处理Todo
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("MaJCalc.db");
    if (!db.open()){
        QMessageBox::information(this, "错误", "数据库打开失败");
    }
    db.transaction();
    QSqlQuery q;
    for(int i = 0 ; i < 4; i ++){
        q.exec(QString("INSERT INTO gameHistory VALUES('%1', '%2', %3, %4, %5)").arg(currentDate).arg(rank[i]).arg(i + 1).arg(result.getPoint(rank[i])).arg(calcMoney(i + 1, result.getPoint(rank[i]))));//date player rank point
    }
    db.commit();
}

void MainWindow:: settingsWindow_ConfirmButtonClicked(QStringList list){
    resultAddWindow -> modifyPlayerName(list);
    bigEventWindow -> setComboName(list);
}

void MainWindow:: databaseSetup(){
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("MaJCalc.db");
    if (!db.open()){
        QMessageBox::information(this, "错误", "数据库打开失败\n" + db.lastError().text());
    }
    QSqlQuery q;
    q.exec("SELECT * FROM sqlite_master WHERE name = 'gameHistory'");
    if (!q.next()){
        q.exec("CREATE TABLE gameHistory(date TEXT NOT NULL, player TEXT NOT NULL, rank INT NOT NULL, point INT NOT NULL, money REAL)");
    }
    q.exec("SELECT * FROM sqlite_master WHERE name = 'rankSystem'");
    if (!q.next()){
        q.exec("CREATE TABLE rankSystem(player TEXT PRIMARY KEY NOT NULL, rank TEXT NOT NULL, point INT NOT NULL)");
    }
    q.exec("SELECT * FROM sqlite_master WHERE name = 'bigEvent'");
    if (!q.next()){
        q.exec("CREATE TABLE bigEvent(date TEXT NOT NULL, lead TEXT, victim TEXT, detail TEXT NOT NULL)");
    }
}

void MainWindow:: mainWindow_modifyButtonClicked(){
    if (ui -> pointTable -> currentRow() == -1)
        return;
    QString date = ui -> pointTable -> item(ui -> pointTable -> currentRow(), 0) -> text();
    emit mainWindow_ResultModifyButtonClickedSignal(date);
}

void MainWindow:: resultAddWindow_ConfirmButtonClicked_Modify(HanCyan result){
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("MaJCalc.db");
    if (!db.open()){
        QMessageBox::information(this, "错误", "数据库打开失败");
    }
    db.transaction();
    QSqlQuery q;
    QString date = result.getGameTime().toString("yyyy年MM月dd日 hh:mm");
    int rowCount = ui -> pointTable -> rowCount();
    for(int i = 0; i < rowCount; i ++){
        if (date == ui -> pointTable -> item(i, 0) -> text()){
            QStringList rank = result.getRank();
            for(int j = 0; j < 4; j ++){
                ui -> pointTable -> setItem(i, j + 1, new QTableWidgetItem(rank[j] + " " + QString:: number(result.getPoint(rank[j]))));
                ui -> pointTable -> item(i, j + 1) -> setTextAlignment(Qt::AlignHCenter | Qt:: AlignVCenter);
                q.exec(QString("UPDATE gameHistory SET rank = %1, point = %2, money = %5 WHERE date = '%3' AND player = '%4'").arg
                       (j + 1).arg(result.getPoint(rank[j])).arg(date).arg(rank[j]).arg(calcMoney(j + 1, result.getPoint(rank[j]))));//date player rank point
                qDebug() << "main_modify" <<q.lastError();
            }
            updateMoney();
            break;
        }
    }
    db.commit();
}

void MainWindow:: mainWindow_DeleteButtonClicked(){
    if (ui -> pointTable -> currentRow() == -1)
        return;
    QString date = ui -> pointTable -> item(ui -> pointTable -> currentRow(), 0) -> text();
    int ret = QMessageBox:: question(this, "警告", QString("是否确认要删除时间为%1的对局记录？该操作不可恢复！").arg(date));
    if (ret == QMessageBox:: Yes){
        ui -> pointTable -> removeRow(ui -> pointTable -> currentRow());
        QSqlQuery q;
        q.exec(QString("DELETE FROM gameHistory WHERE date = '%1'").arg(date));
        updateMoney();
    }
}

double MainWindow:: calcMoney(int rank, int point){
    QStringList uma = settingsWindow -> getUma();
    int kaeshi = settingsWindow -> getKaeshi(), rate = settingsWindow -> getRate();
    return double(point - kaeshi) / rate + uma[rank - 1].toInt();
}
