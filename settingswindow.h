#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QVector>
#include <QSettings>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

    QStringList getUma();
    int getRate();
    QStringList getPlayerList();
    int getMochi();
    int getKaeshi();

private:
    Ui::SettingsWindow *ui;
    QLineEdit *uma[4];
    void closeEvent ( QCloseEvent *e);
    void restoreSettings();

public slots:
    void mainWindow_SettingsWindowClicked();
    void settingsWindow_ConfirmButtonClicked();
    void settingsWindow_CancelButtonClicked();

signals:
    void settingsWindow_ConfirmButtonClickedSignal(QStringList list);

};

#endif // SETTINGSWINDOW_H
