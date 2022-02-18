#ifndef BIGEVENTWINDOW_H
#define BIGEVENTWINDOW_H

#include <QMainWindow>

namespace Ui {
class BigEventWindow;
}

class BigEventWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BigEventWindow(QWidget *parent = nullptr);
    ~BigEventWindow();

private:
    Ui::BigEventWindow *ui;
};

#endif // BIGEVENTWINDOW_H
