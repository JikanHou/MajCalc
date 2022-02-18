#ifndef GAMEHISTORYWINDOW_H
#define GAMEHISTORYWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCalendarWidget>
#include <QMessageBox>
#include "hancyan.h"


namespace Ui {
    class GameHistoryWindow;
}

class GameHistoryWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit GameHistoryWindow(QWidget *parent = nullptr);
    ~GameHistoryWindow();

private:
    Ui::GameHistoryWindow *ui;

public slots:
    void calenderModified();
    void mainWindow_CalendarButtonClicked();
};

#endif // GAMEHISTORYWINDOW_H
