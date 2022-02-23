#ifndef RANKSYSTEM_H
#define RANKSYSTEM_H
#include <QString>
#include "hancyan.h"


class RankSystem{

private:
    QString name;
    int rank_lvl;
    int pt, ptMax;

public:
    RankSystem(QString name = "");
	RankSystem(QString name, QString rank, int pt);

	void setRank(int rank);

	void lvlUp();
    void lvlDown();

	QString getRank();
    int getPt();
    int getPtMax();
    int getIntRank();

	void gameFinished(int rank, int point);
};

#endif // RANKSYSTEM_H
