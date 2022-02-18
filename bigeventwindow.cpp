#include "bigeventwindow.h"
#include "ui_bigeventwindow.h"

BigEventWindow::BigEventWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BigEventWindow)
{
    ui->setupUi(this);
}

BigEventWindow::~BigEventWindow()
{
    delete ui;
}
