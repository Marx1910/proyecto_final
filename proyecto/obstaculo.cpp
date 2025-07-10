#include "obstaculo.h"
#include <QTimer>
#include <QPixmap>
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QDebug>

Obstaculo::Obstaculo(Tipo tipo, QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent),
    velocidad(10), tipoObstaculo(tipo), animacion(nullptr) {

    configurarAnimacion();
    configurarPosicionInicial();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Obstaculo::mover);
    timer->start(30);
}

void Obstaculo::configurarAnimacion() {
    QString rutaSpritesheet;
    int anchoFrame, altoFrame;
    int filaAnimacion, frameInicio, frameFin, fps;

    switch(tipoObstaculo) {
    case NAVE:
        rutaSpritesheet = ":/new/prefix1/recursos/nave.png";
        anchoFrame = 22;
        altoFrame = 24;
        filaAnimacion = 0;  // Fila en el spritesheet
        frameInicio = 0;    // Frame inicial
        frameFin = 6;      // 4 frames de animación
        fps = 30;          // Velocidad animación
        break;

    case PERRO:
        rutaSpritesheet = ":/new/prefix1/recursos/perro.png";
        anchoFrame = 39;
        altoFrame = 28;
        filaAnimacion = 0;
        frameInicio = 0;
        frameFin = 5;      // 6 frames de animación
        fps = 30;
        break;

        setScale(0.5);
        //setTransformOriginPoint(boundingRect().center()); // Centrar el escalado
        break;
    }

    QPixmap spriteSheet(rutaSpritesheet);
    if(spriteSheet.isNull()) {
        qDebug() << "Error: No se pudo cargar spritesheet para" << tipoObstaculo;
        return;
    }

    animacion = new AnimacionSprite(this, spriteSheet, anchoFrame, altoFrame, this);
    animacion->configurarSecuencia(filaAnimacion, frameInicio, frameFin, fps);
    animacion->reproducir();
}

void Obstaculo::configurarPosicionInicial() {
    if (!scene()) {
        qDebug() << "Error: Obstáculo no está en una escena";
        return;
    }

    qreal yPos;
    const qreal sceneHeight = scene()->height();

    switch(tipoObstaculo) {
    case NAVE:
        yPos = sceneHeight * (0.3 + 0.3 * QRandomGenerator::global()->generateDouble());
        break;
    case PERRO:
        yPos = scene()->height() - 120 - (boundingRect().height() * 0.5);
        break;
    }

    setPos(scene()->width(), yPos);
    qDebug() << "Obstáculo posicionado en Y:" << yPos;
}

void Obstaculo::setVelocidad(int velocidad) {
    this->velocidad = velocidad;
}

int Obstaculo::getVelocidad() const {
    return velocidad;
}

Obstaculo::Tipo Obstaculo::getTipo() const {
    return tipoObstaculo;
}

void Obstaculo::mover() {
    setPos(x() - velocidad, y());

    // Eliminar cuando sale de pantalla
    if(pos().x() + boundingRect().width() < 0) {
        scene()->removeItem(this);
        delete this;
    }
}
