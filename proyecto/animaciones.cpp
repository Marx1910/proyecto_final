#include "animaciones.h"

AnimacionSprite::AnimacionSprite(QGraphicsPixmapItem *target,
                                 QPixmap spriteSheet,
                                 int anchoFrame,
                                 int altoFrame,
                                 QObject *parent)
    : QObject(parent),
    destino(target),
    hoja(spriteSheet),
    ancho(anchoFrame),
    alto(altoFrame),
    filaActual(0),
    frameInicio(0),
    frameFin(0),
    frameActual(0)
{
    connect(&temporizador, &QTimer::timeout, this, &AnimacionSprite::actualizarFrame);
}

void AnimacionSprite::configurarSecuencia(int fila, int inicio, int fin, int fps)
{
    filaActual = fila;
    frameInicio = inicio;
    frameFin = fin;
    frameActual = frameInicio;
    temporizador.setInterval(1000 / fps);
}

void AnimacionSprite::reproducir()
{
    temporizador.start();
}

void AnimacionSprite::detener()
{
    temporizador.stop();
}

void AnimacionSprite::actualizarFrame()
{
    int x = frameActual * ancho;
    int y = filaActual * alto;

    QPixmap frame = hoja.copy(x, y, ancho, alto).scaled(
        ancho * 2,
        alto * 2,
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation
        );

    destino->setPixmap(frame);
    destino->setTransformOriginPoint(frame.width() / 2, frame.height());

    frameActual = (frameActual + 1 > frameFin) ? frameInicio : frameActual + 1;
}
