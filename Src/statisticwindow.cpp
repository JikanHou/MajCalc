#include "statisticwindow.h"
#include "ui_statisticwindow.h"

StatisticWindow::StatisticWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui:: StatisticWindow){
    ui->setupUi(this);
    connect(ui -> playerName, &QComboBox:: currentTextChanged, this, &StatisticWindow:: playerChosen);
    connect(ui -> statcTime, &QComboBox:: currentTextChanged, this, &StatisticWindow:: playerChosen);
    playerChosen();
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");
    if (!db.open()){
        QMessageBox::information(this, "错误", "数据库打开失败");
        return;
    }
}

StatisticWindow::~StatisticWindow(){
    delete ui;
}

void StatisticWindow:: mainWindow_StatisticButtonClicked(){
    setWindowModality(Qt:: ApplicationModal);
    show();

    QSqlQuery q;
    q.exec(QString("SELECT DISTINCT player FROM gameHistory"));//date player rank point
    qDebug() << "stat clicked" << q.lastError();
    QStringList list;
    while (q.next()){
        list << q.value(0).toString();
    }
    ui -> playerName -> clear();
    ui -> playerName -> addItems(list);
}

void StatisticWindow:: playerChosen(){
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");
    if (!db.open()){
        QMessageBox::information(this, "错误", "数据库打开失败");
        return;
    }
    QSqlQuery q;
    if (ui -> playerName -> currentText().isEmpty()){
        ui -> totalGameCount -> setText("0");
        ui -> ichii -> setText("NaN");
        ui -> nii -> setText("NaN");
        ui -> sani -> setText("NaN");
        ui -> yoni -> setText("NaN");
        ui -> avgRank -> setText("NaN");
        ui -> highest -> setText("0");
        ui -> lowest -> setText("0");
        ui -> avgPoint -> setText("NaN");
        ui -> flyingRate -> setText("NaN");
        ui -> avgMoney -> setText("NaN");
        ui -> sumMoney -> setText("0");
        ui -> rankPic -> setPixmap(QPixmap(QString(":/Icons/rankDefault.png")));
        ui -> pt -> setText("INF/INF\n雀渣");
    }
    else{
        //qDebug() << QString::fromLocal8Bit(ui -> playerName -> currentText().toLocal8Bit()) << ui -> statcTime -> currentText();
        int rank[4] = {0, 0, 0, 0}, sumPoint = 0, gameCnt = 0, maxPoint = -100000, minPoint = 100000;
        int tobi = 0;
        double sumMoney = 0;
        QString timeDuration = ui -> statcTime -> currentText();
        QDateTime now = QDateTime:: currentDateTime();
        int targetCnt = 0; QDateTime targetTime;
        if (timeDuration == "近7天" || timeDuration == "近30天" || timeDuration == "近365天"){
            if (timeDuration == "近7天")
                targetTime = now.addDays(-7);
            else if (timeDuration == "近30天")
                targetTime = now.addDays(-30);
            else
                targetTime = now.addDays(-365);
            q.exec(QString("SELECT rank, point, money FROM gameHistory WHERE player = '%1' and date >= '%2'").arg(ui -> playerName -> currentText()).arg(targetTime.toString("yyyy年MM月dd日 hh:mm")));
            targetCnt = 0x7ffffff;
        }
        else{
            q.exec(QString("SELECT rank, point, money, date FROM gameHistory WHERE player = '%1' ORDER BY date DESC").arg(ui -> playerName -> currentText()));
            if (timeDuration == "近30场")
                targetCnt = 30;
            else if (timeDuration == "近50场")
                targetCnt = 50;
            else if (timeDuration == "近100场")
                targetCnt = 100;
            else if (timeDuration == "生涯")
                targetCnt = 0x7ffffff;
        }
        //qDebug() << "statistic" << q.lastError() << targetTime << targetCnt;
        //qDebug() << q.lastQuery();
        while (q.next() && targetCnt --){
            int r = q.value(0).toInt(), point = q.value(1).toInt();
            double money = q.value(2).toDouble();
            qDebug() << r << point << money;
            gameCnt ++;
            sumPoint += point;
            rank[r - 1] ++;
            if (maxPoint < point)
                maxPoint = point;
            if (minPoint > point)
                minPoint = point;
            if (point < 0)
               tobi ++;
            sumMoney += money;
        }
        if (gameCnt == 0){
            ui -> totalGameCount -> setText(QString:: number(gameCnt));
            ui -> ichii -> setText("NaN");
            ui -> nii -> setText("NaN");
            ui -> sani -> setText("NaN");
            ui -> yoni -> setText("NaN");
            ui -> avgRank -> setText("NaN");
            ui -> highest -> setText(0);
            ui -> lowest -> setText(0);
            ui -> avgPoint -> setText("NaN");
            ui -> flyingRate -> setText("NaN");
            ui -> avgMoney -> setText("NaN");
            ui -> sumMoney -> setText(0);
        }
        else{
            ui -> totalGameCount -> setText(QString:: number(gameCnt));
            ui -> ichii -> setText(QString:: number(double(rank[0]) / gameCnt, 'f', 2));
            ui -> nii -> setText(QString:: number(double(rank[1]) / gameCnt, 'f', 2));
            ui -> sani -> setText(QString:: number(double(rank[2]) / gameCnt, 'f', 2));
            ui -> yoni -> setText(QString:: number(double(rank[3]) / gameCnt, 'f', 2));
            ui -> avgRank -> setText(QString:: number(double(rank[0] + 2 * rank[1] + 3 * rank[2] + 4 * rank[3]) / gameCnt, 'g', 2));
            ui -> highest -> setText(QString:: number(maxPoint));
            ui -> lowest -> setText(QString:: number(minPoint));
            ui -> avgPoint -> setText(QString:: number(sumPoint / gameCnt));
            ui -> flyingRate -> setText(QString:: number(double(tobi) / gameCnt, 'f', 2));
            ui -> avgMoney -> setText(QString:: number(sumMoney / gameCnt, 'f', 2));
            ui -> sumMoney -> setText(QString:: number(sumMoney));
        }
        q.exec(QString("SELECT rank, point, money, date FROM gameHistory WHERE player = '%1' ORDER BY date ASC").arg(ui -> playerName -> currentText()));
        RankSystem rs(ui -> playerName -> currentText());
        while (q.next()){
            int rank = q.value(0).toInt(), point = q.value(1).toInt();
            rs.gameFinished(rank, point);
        }
        ui -> pt -> setText(QString("%1/%2\n%3").arg(rs.getPt()).arg(rs.getPtMax()).arg(rs.getRank()));
        ui -> rankPic -> setPixmap(QPixmap(QString(":/Icons/rank%1.png").arg(rs.getIntRank())));
    }
}
