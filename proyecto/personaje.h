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

    // Métodos virtuales puros (deben ser implementados por clases derivadas)
    virtual void saltar() = 0;
    virtual void agacharse() = 0;
    virtual void levantarse() = 0;
    virtual void actualizar() = 0;

    // Métodos comunes
    Estado estadoActual() const;
    bool estaEnSuelo() const;

protected:
    // Funciones de ayuda para física
    void aplicarGravedad();
    void verificarColisionSuelo();

    // Estado del personaje
    Estado estado;
    bool enSuelo;
    bool agachado;

    // Propiedades físicas
    qreal velocidadY;
    qreal gravedad;
    qreal fuerzaSalto;

    // Temporizador para actualizaciones
    QTimer *timerFisica;
};

#endif // PERSONAJE_H
