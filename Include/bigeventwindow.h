#ifndef BIGEVENTWINDOW_H
#define BIGEVENTWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QTableWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQueryModel>

namespace Ui {
class BigEventWindow;
}

class BigEventWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit BigEventWindow(QWidget *parent = nullptr);
    ~BigEventWindow();
    void setComboName(const QStringList &list);
public slots:
    void mainWindow_bigEventButtonClicked();

private slots:
    void queryButtonClicked();
    void addButtonClicked();
    void delButtonClicked_Q();
    void submitButtonClicked();
    void comboBoxModified();
    void calendarClicked();
    void clearButtonClicked();
    void delButtonClicked();

private:
    Ui::BigEventWindow *ui;
};

#endif // BIGEVENTWINDOW_H
