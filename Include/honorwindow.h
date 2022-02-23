#ifndef HONORWINDOW_H
#define HONORWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QStringList>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
	class HonorWindow;
}

struct Node{
	int rank[4], point;
	double money;
	Node(){
		memset(rank, 0, sizeof(rank));
		point = money = 0;
	}
};

class HonorWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit HonorWindow(QWidget *parent = nullptr);
    ~HonorWindow();

private:
	Ui:: HonorWindow *ui;
	QSqlDatabase db;

	void updatePoint();
	void updateRandomEvent();
	void updateRecentEvent();

public slots:
	void mainWindow_HonorWindowButtonClicked();
};

#endif // HONORWINDOW_H
