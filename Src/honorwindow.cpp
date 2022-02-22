#include "honorwindow.h"
#include "ui_honorwindow.h"

HonorWindow::HonorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HonorWindow)
{
    ui->setupUi(this);
}

HonorWindow::~HonorWindow()
{
    delete ui;
}
