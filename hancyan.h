#ifndef HANCYAN_H
#define HANCYAN_H

#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QPair>
#include <QMap>

class HanCyan{
public:
    HanCyan();
    bool addPlayer(const QString &Player, int Point);
    QStringList getRank();
    int getPoint(const QString &name);
    void setGameTime();
    void setGameTime(QDateTime time);
    QDateTime getGameTime();
    int size();

private:
    QMap<QString, int> result;
    QDateTime gameTime;
};

#endif // HANCYAN_H
