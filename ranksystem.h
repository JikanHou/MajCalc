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
    void setRank(int rank);
    void lvlUp();
    void lvlDown();
    QString getRank();
    int getPt();
    int getPtMax();
    void gameFinished(int rank, int point);
    int getIntRank();
};

#endif // RANKSYSTEM_H
