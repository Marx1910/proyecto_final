#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Personaje : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    enum Estado { Quieto, Corriendo, Saltando, Cayendo, Agachado };

    explicit Personaje(QGraphicsItem *parent = nullptr);
    virtual ~Personaje();

    virtual void saltar() = 0;
    virtual void agacharse() = 0;
    virtual void levantarse() = 0;
    virtual void actualizar() = 0;

    Estado estadoActual() const;
    bool estaEnSuelo() const;

    // Nuevo setter/getter para offset de suelo
    void setGroundOffset(int off) { groundOffset = off; }
    int  getGroundOffset() const    { return groundOffset; }

protected:
    void aplicarGravedad();
    void verificarColisionSuelo();

    Estado estado;
    bool enSuelo;
    bool agachado;

    qreal velocidadY;
    qreal gravedad;
    qreal fuerzaSalto;

    int groundOffset;      // distancia desde bottom hasta el “suelo”

    QTimer *timerFisica;
};

#endif // PERSONAJE_H
