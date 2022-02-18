#include "ranksystem.h"

static QString rank_cn[16];
static int bonus[4][16];
static int ptMaxList[16];
static int ptStart[16];

RankSystem::RankSystem(QString name){
    this -> name = name;
    QString list[] = {"初心%1星", "雀士%1星", "雀杰%1星", "雀豪%1星", "雀圣%1星"};
    int c = 0;
    for(int i = 0; i < 5; i ++){
        for(int j = 1; j <= 3; j ++){
            rank_cn[c ++] = list[i].arg(j);
        }
    }
    rank_cn[c] = "魂天";
    int pml[] = {20, 80, 200, 600, 800, 1000, 1200, 1400, 2000, 2800, 3200, 3600, 4000, 6000, 9000, 10000000};
    for(int i = 0; i < 16; i ++){
        ptMaxList[i] = pml[i];
        if (pml[i] <= 200)
           ptStart[i] = 0;
        else
           ptStart[i] = pml[i] / 2;
    }
    int room_b[] = {10, 20, 40, 55, 60, 0};
    int room_d[] = {-10, -10, -10, -30, -50, -70, -90, -110, -130, -175, -190, -205, -220, -235, -250, 0};
    for(int i = 0; i < 16; i ++){//10, 20, 40 ,55, 60
        bonus[2][i] = -5;
        bonus[1][i] = room_b[i / 3] + 5;
        bonus[0][i] = room_b[i / 3] * 2 + 15;
        bonus[3][i] = room_d[i] - 5;
    }
    rank_lvl = 0;
    pt = ptStart[0]; ptMax = ptMaxList[0];

}

void RankSystem:: gameFinished(int rank, int point){
    int newPt = bonus[rank - 1][rank_lvl];
    newPt += (point - 25000) / 1000;
    pt += newPt;
    if (pt >= ptMax)
        lvlUp();
    if (pt < 0)
        lvlDown();
}

void RankSystem:: lvlUp(){
    rank_lvl ++;
    ptMax = ptMaxList[rank_lvl];
    pt = ptStart[rank_lvl];
}

void RankSystem:: lvlDown(){
    if (rank_lvl >= 3)
        rank_lvl --;
    ptMax = ptMaxList[rank_lvl];
    pt = ptStart[rank_lvl];
}

int RankSystem:: getPt(){
    return pt;
}

int RankSystem:: getPtMax(){
    return ptMax;
}

QString RankSystem:: getRank(){
    return rank_cn[rank_lvl];
}

int RankSystem:: getIntRank(){
    return rank_lvl;
}
