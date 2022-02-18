#ifndef RESULTADDWINDOW_H
#define RESULTADDWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "hancyan.h"

namespace Ui {
class ResultAddWindow;
}
enum addWindowMode{
    add, modify
};

class ResultAddWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ResultAddWindow(QWidget *parent = nullptr);
    ~ResultAddWindow();
    void modifyPlayerName(QStringList list);

private:
    Ui::ResultAddWindow *ui;
    QComboBox *playerName[4];
    QLineEdit *playerPoint[4];
    addWindowMode mode;
    QString currentTime;

signals:
    void resultAddWindow_CancelButtonClickedSignal();
    void resultAddWindow_ConfirmButtonClickedSignal(HanCyan result);
    void resultAddWindow_ConfirmButtonClickedSignal_Modify(HanCyan result);

public slots:
    void resultAddWindow_CancelButtonClicked();
    void resultAddWindow_ConfirmButtonClicked();
    void mainWindow_ResultAddButtonClicked();
    void mainWindow_ResultModifyButtonClicked(QString date);
    void pointsModified();
};

#endif // RESULTADDWINDOW_H
