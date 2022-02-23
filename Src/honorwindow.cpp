#include "honorwindow.h"
#include "ui_honorwindow.h"

HonorWindow::HonorWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::HonorWindow){
	ui -> setupUi(this);
	if(QSqlDatabase::contains("qt_sql_default_connection"))
	  db = QSqlDatabase::database("qt_sql_default_connection");
	else
	  db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("MaJCalc.db");
	if (!db.open()){
		QMessageBox::information(this, "错误", "数据库打开失败");
	}
	//updatePoint();
	//updateRandomEvent();
	//updateRecentEvent();
}

HonorWindow::~HonorWindow(){
    delete ui;
}

void HonorWindow:: updatePoint(){
	ui -> honorList -> clear();
	ui -> shameList -> clear();
	bool hasRec = false;
	QSqlQuery q;
	QMap<QString, Node> map;
	int INF = INT_MAX;
	int minPoint = INF, maxPoint = -INF, minGame = INF, maxGame = -INF, minAvg = INF, maxAvg = -INF;
	double minRentai = INF, maxRentai = -INF, minMoney = INF, maxMoney = -INF, minRank = INF, maxRank = -INF;
	double minAvgMoney = INF, maxAvgMoney = -INF;
	q.exec("SELECT player, point, rank, money from gameHistory");

	while (q.next()){
		hasRec = true;
		QString name = q.value(0).toString();
		map[name].point += q.value(1).toInt();
		map[name].rank[q.value(2).toInt() - 1] ++;
		map[name].money += q.value(3).toDouble();
	}
	if (!hasRec){
		setWindowModality(Qt:: NonModal);
		return;
	}
	QString minPointName, maxPointName, minRentaiName, maxRenTaiName, minAvgName, maxAvgName, minMoneyName, maxMoneyName, minGameName, maxGameName, minRankName, maxRankName;
	QString minAvgMoneyName, maxAvgMoneyName;
	for (QMap<QString, Node>:: iterator iter = map.begin(); iter != map.end(); ++ iter){
		Node &it = iter.value();
		double rank, rentai = it.rank[0] + it.rank[1], sum = it.rank[0] + it.rank[1] + it.rank[2] + it.rank[3];
		rank = it.rank[0] + it.rank[1] * 2 + it.rank[2] * 3 + it.rank[3] * 4;
		rank /= sum;
		//平均顺位
		if (rank > maxRank){
			maxRank = rank;
			maxRankName = iter.key();
		}
		if (rank < minRank){
			minRank = rank;
			minRankName = iter.key();
		}

		//连对率
		rentai /= sum;
		if (rentai < minRentai){
			minRentai = rentai;
			minRentaiName = iter.key();
		}
		if (rentai > maxRentai){
			maxRentai = rentai;
			maxRenTaiName = iter.key();
		}
	}

	//最大对局场数
	q.exec(QString("SELECT player, count(*) FROM gameHistory GROUP BY player"));
	while (q.next()){
		int count = q.value(1).toInt();
		if (count > maxGame){
			maxGame = count;
			maxGameName = q.value(0).toString();
		}
		if (count < minGame){
			minGame = count;
			minGameName = q.value(0).toString();
		}
	}

	//平均分
	q.exec("SELECT player, avg(point) FROM gameHistory GROUP BY player");
	while (q.next()){
		int avg = q.value(1).toDouble();
		if (avg > maxAvg){
			maxAvg = avg;
			maxAvgName = q.value(0).toString();
		}
		if (avg < minAvg){
			minAvg = avg;
			minAvgName = q.value(0).toString();
		}
	}

	//最大最小分数
	q.exec("SELECT player, point FROM gameHistory WHERE point = (SELECT max(point) FROM gameHistory)");
	q.next();
	maxPoint = q.value(1).toInt();
	maxPointName = q.value(0).toString();
	q.exec("SELECT player, point FROM gameHistory WHERE point = (SELECT min(point) FROM gameHistory)");
	q.next();
	minPoint = q.value(1).toInt();
	minPointName = q.value(0).toString();
	q.exec("SELECT player, am FROM (SELECT player, avg(money) as am, row_number() over (order by avg(money) DESC) as rn FROM gameHistory GROUP BY player) WHERE rn = 1");
	q.next();
	maxAvgMoney = q.value(1).toDouble();
	maxAvgMoneyName = q.value(0).toString();
	q.exec("SELECT player, am FROM (SELECT player, avg(money) as am, row_number() over (order by avg(money) ASC) as rn FROM gameHistory GROUP BY player) WHERE rn = 1");
	q.next();
	minAvgMoney = q.value(1).toDouble();
	minAvgMoneyName = q.value(0).toString();


	//总收益
	q.exec("SELECT player, sum(money) FROM gameHistory GROUP BY player");
	while (q.next()){
		double money = q.value(1).toDouble();
		if (money > maxMoney){
			maxMoneyName = q.value(0).toString();
			maxMoney = q.value(1).toDouble();
		}
		if (money < minMoney){
			minMoneyName = q.value(0).toString();
			minMoney = q.value(1).toDouble();
		}
	}

	srand(time(NULL));
	QListWidget *list[2];
	list[0] = ui -> honorList; list[1] = ui -> shameList;
	int x;
	QString s;
	//最高分最低分
	list[0] -> addItem(QString("太强辣，%2在一个半庄中获得了%1的分数，其他人苦不堪言！").arg(maxPoint).arg(maxPointName));
	list[1] -> addItem(QString("太菜辣，%2在一个半庄中只剩下%1的分数，其他人喜笑颜开！").arg(minPoint).arg(minPointName));

	//最高平均分
	x = rand() % 2;

	if (x == 0)
		s = "太强辣，%2每个半庄平均有%1点，其他玩家都在干什么啊！";
	else
		s = "真辣鸡，%2每个半庄平均有%1点，真是一毛不拔！";
	list[x] -> addItem(s.arg(maxAvg).arg(maxAvgName));

	//最低平均分
	x = rand() % 2;
	if (x == 0)
		s = "太强辣，%2每个半庄平均只有%1点，其他玩家真是收益颇丰！";
	else
		s = "真辣鸡，%2每个半庄平均只有%1点，建议回家沐浴焚香！";
	list[x] -> addItem(s.arg(minAvg).arg(minAvgName));

	//最高总计收益
	x = rand() % 2;
	if (x == 0)
		s = "可恶！%2已经在牌桌上白嫖了%1场轮抓！";
	else
		s = "哎呀！%2已经在牌桌上打回来了%1场轮抓的钱，怎么在轮抓桌子上看不见他的影子呢？";
	list[x] -> addItem(s.arg(maxMoney / 85).arg(maxMoneyName));

	//最低总计收益
	x = rand() % 2;
	if (x == 0)
		s = "莫非%2就是传说中的慈善赌王？一共接济了穷人%1块钱！";
	else
		s = "强烈建议%2把输掉的一共%1块钱用来打轮抓！";
	list[x] -> addItem(s.arg(-minMoney).arg(minMoneyName));


	//最高场均收益
	x = rand() % 2;
	if (x == 0)
		s = "%2每个半庄平均收获了%1块钱，建议请客吃饭！";
	else
		s = "%2居然在每个半庄平均收获了%1块钱，真是世风日下！";

	list[x] -> addItem(s.arg(maxAvgMoney).arg(maxAvgMoneyName));

	//最低场均收益
	x = rand() % 2;
	if (x == 0)
		s = "%2平均每个半庄付出了%1块钱帮助他人改善生活，致敬！";
	else
		s = "%2平均每个半庄支出%1块钱，陷入破产边缘。";
	list[x] -> addItem(s.arg(-minAvgMoney).arg(minAvgMoneyName));

	//最高连对率
	x = rand() % 2;
	if (x == 0)
		s = "%2的连对率高达%1%!这是人做的事？";
	else
		s = "%2由于在%1%的半庄中都没有支付水钱，已被警方抓获。";
	list[x] -> addItem(s.arg(maxRentai * 100).arg(maxRenTaiName));

	//最低连对率
	x = rand() % 2;
	if (x == 0)
		s = "%2支付了%1%半庄的水钱，感谢这名同志为聚卡麻酱馆做出的贡献！";
	else
		s = "%2在%1%半庄中都出了水钱，整天粘在麻酱桌上下不去！";
	list[x] -> addItem(s.arg((1 - minRentai) * 100).arg(minRentaiName));

	//最大对局场数
	x = rand() % 2;
	if (x == 0)
		s = "%2共计打了%1个半庄，可谓当之无愧的麻坛不老松！";
	else
		s = "%2共计虚度了%1个半庄的光阴，见鬼，他不用上班的吗？";
	list[x] -> addItem(s.arg(maxGame).arg(maxGameName));

}

void HonorWindow:: mainWindow_HonorWindowButtonClicked(){
	setWindowModality(Qt:: ApplicationModal);
	show();
	updatePoint();
	updateRandomEvent();
	updateRecentEvent();
}

void HonorWindow:: updateRandomEvent(){

}

void HonorWindow:: updateRecentEvent(){

}
