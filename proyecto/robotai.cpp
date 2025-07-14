#include "robotai.h"
#include "robot.h"
#include "goku.h"
#include <cmath>
#include <QDebug>

RobotAI::RobotAI(Robot *enemy, Goku *player, QObject *parent)
    : QObject(parent),
    robot(enemy),
    goku(player)
{
    robot->setTarget(goku);

    attackCooldown = new QTimer(this);
    attackCooldown->setInterval(1000);
    attackCooldown->setSingleShot(true);

    timer = new QTimer(this);
    timer->setInterval(200);
    connect(timer, &QTimer::timeout,
            this, &RobotAI::evaluar);
}

void RobotAI::start()
{
    timer->start();
}

void RobotAI::evaluar()
{
    if (!robot || !goku) return;

    qreal dx   = goku->x() - robot->x();
    qreal dy   = goku->y() - robot->y();
    qreal dist = std::sqrt(dx*dx + dy*dy);

    // Ataque cuerpo a cuerpo o rayo
    if (dist < 80 && !attackCooldown->isActive()) {
        if (qrand() % 2 == 0)
            robot->golpear();
        else {
            int dir = (dx < 0 ? -1 : +1);
            robot->lanzarRayo(dir);
        }
        attackCooldown->start();
        robot->movingLeft  = false;
        robot->movingRight = false;
        return;
    }

    // Mover hacia Gokú
    if (dx > 5) {
        robot->movingRight = true;
        robot->movingLeft  = false;
    }
    else if (dx < -5) {
        robot->movingLeft  = true;
        robot->movingRight = false;
    }
    else {
        robot->movingLeft  = false;
        robot->movingRight = false;
    }

    // A veces salta
    if (robot->isOnGround() && qAbs(dx) < 150 && qrand() % 10 == 0)
        robot->saltar();
}
