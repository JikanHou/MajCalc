#ifndef STATISTICWINDOW_H
#define STATISTICWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QStringList>
#include <QLabel>
#include <QComboBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "hancyan.h"
#include "ranksystem.h"
namespace Ui {
class StatisticWindow;
}

class StatisticWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StatisticWindow(QWidget *parent = nullptr);
    ~StatisticWindow();

    Ui::StatisticWindow *ui;

private:
public slots:
    void mainWindow_StatisticButtonClicked();
    void playerChosen();
};

#endif // STATISTICWINDOW_H
