// obstaculo.h
#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include "animaciones.h"

class Obstaculo : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    enum Tipo {
        NAVE,    // Obstáculo volador
        PERRO,   // Obstáculo terrestre
    };

    Obstaculo(Tipo tipo, QGraphicsItem *parent = nullptr);
    void setVelocidad(int velocidad);
    int getVelocidad() const;
    Tipo getTipo() const;

public slots:
    void mover();

private:
    int velocidad;
    Tipo tipoObstaculo;
    AnimacionSprite* animacion;

    void configurarAnimacion();
    void configurarPosicionInicial();

};

#endif // OBSTACULO_H
