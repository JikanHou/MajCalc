#include "hancyan.h"

HanCyan::HanCyan(){

}

bool HanCyan:: addPlayer(const QString &player, int point){
    if (result.size() >= 4)
        return false;
    result[player] = point;
    return true;
}

int HanCyan:: getPoint(const QString &name){
    if (!result.count(name))
        return -1;
    return result[name];
}

QVector<QString> HanCyan:: getRank(){
    QVector <QString> nameList;
    if (result.size() == 4){
        for (auto it = result.begin(); it != result.end(); ++ it){
            nameList.append(it.key());
        }
        for (int i = 0; i < 4; i ++)
            for (int j = 0; j + i < 3; j ++)
                if (result[nameList[j]] < result[nameList[j + 1]])
                    swap(nameList[j], nameList[j + 1]);

    }
    return nameList;
}

void HanCyan:: setGameTime(){
    gameTime = QDateTime:: currentDateTime();
}

void HanCyan:: setGameTime(QDateTime time){
    gameTime = time;
}

QDateTime HanCyan:: getGameTime(){
    return gameTime;
}

int HanCyan:: size(){
    return result.size();
}
