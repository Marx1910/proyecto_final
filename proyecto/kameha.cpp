#include "kameha.h"
#include "robot.h"
#include "goku.h"
#include "personaje.h"
#include <QGraphicsScene>

Kamehameha::Kamehameha(int direction,
                       Personaje* owner,
                       QGraphicsScene* scene,
                       QObject* parent)
    : QObject(parent),
    QGraphicsPixmapItem(),
    direction(direction),
    owner(owner)
{
    // Sprite del proyectil
    setPixmap(QPixmap(":/new/prefix1/recursos/Kamehameha.png"));
    setScale(0.8);
    setZValue(50);

    // Añadir a la escena
    if (scene) scene->addItem(this);

    // Timer de movimiento y colisiones
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Kamehameha::mover);
    timer->start(30);
}

Kamehameha::~Kamehameha()
{
    timer->stop();
}

void Kamehameha::mover()
{
    // 1) Desplaza el rayo
    setX(x() + direction * speed);

    // 2) Si sale de los límites, eliminarlo
    if (!scene() ||
        !scene()->sceneRect().intersects(sceneBoundingRect()))
    {
        timer->stop();
        scene()->removeItem(this);
        delete this;
        return;
    }

    // 3) Chequear colisiones
    for (QGraphicsItem* it : collidingItems()) {
        if (it == owner)
            continue;

        // Impacto a Robot
        if (auto *r = dynamic_cast<Robot*>(it)) {
            r->perderVida(1);
        }
        // Impacto a Gokú
        else if (auto *g = dynamic_cast<Goku*>(it)) {
            g->perderVida(1);
        }
        else {
            continue;
        }

        // Tras el impacto, destruir el rayo
        timer->stop();
        scene()->removeItem(this);
        delete this;
        return;
    }
}
