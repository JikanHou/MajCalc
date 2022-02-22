#ifndef HONORWINDOW_H
#define HONORWINDOW_H

#include <QMainWindow>

namespace Ui {
class HonorWindow;
}

class HonorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HonorWindow(QWidget *parent = nullptr);
    ~HonorWindow();

private:
    Ui::HonorWindow *ui;
};

#endif // HONORWINDOW_H
