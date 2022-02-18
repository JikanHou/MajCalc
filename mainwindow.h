#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QLabel>
#include <QTableView>
#include <QAction>
#include <QToolBar>
#include <QDateTime>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTableWidget>
#include "resultaddwindow.h"
#include "hancyan.h"
#include "settingswindow.h"
#include "statisticwindow.h"
#include "gamehistorywindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ResultAddWindow *resultAddWindow;
    SettingsWindow *settingsWindow;
    StatisticWindow *statisticWindow;
    GameHistoryWindow *gameHistoryWindow;

    //QString currentTime;
    QDateTime lastTime;


    void restoreSettings();
    void addRecordFromDB(const QString &date, int &row);
    void updateMoney();
    void databaseSetup();
    double calcMoney(int rank, int point);

public slots:
    void resultAddWindow_ConfirmButtonClicked(HanCyan result);
    void settingsWindow_ConfirmButtonClicked(QStringList list);
    void mainWindow_modifyButtonClicked();
    void resultAddWindow_ConfirmButtonClicked_Modify(HanCyan result);
    void mainWindow_DeleteButtonClicked();

signals:
    void mainWindow_resultUploadButtonClickedSignal();
    void mainWindow_ResultModifyButtonClickedSignal(QString date);

};
#endif // MAINWINDOW_H
