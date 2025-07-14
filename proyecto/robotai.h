#ifndef ROBOTAI_H
#define ROBOTAI_H

#include <QObject>
#include <QTimer>

class Robot;
class Goku;

class RobotAI : public QObject
{
    Q_OBJECT

public:
    explicit RobotAI(Robot *enemy, Goku *player,
                     QObject *parent = nullptr);
    void start();

private slots:
    void evaluar();

private:
    Robot  *robot;
    Goku   *goku;
    QTimer *timer;
    QTimer *attackCooldown;
};

#endif // ROBOTAI_H
