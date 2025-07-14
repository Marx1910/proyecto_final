#include "personaje.h"
#include <QDebug>
#include <QGraphicsScene>

Personaje::Personaje(QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent),
    estado(Quieto),
    enSuelo(true),
    agachado(false),
    velocidadY(0),
    gravedad(0.5),
    fuerzaSalto(-12),
    groundOffset(107),      // offset por defecto (Nivel1)
    timerFisica(new QTimer(this))
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    connect(timerFisica, &QTimer::timeout, this, [this](){
        this->actualizar();
    });
    timerFisica->start(16);
}

Personaje::~Personaje()
{
    if (timerFisica) {
        timerFisica->stop();
        timerFisica->deleteLater();
    }
}

void Personaje::aplicarGravedad()
{
    if (!enSuelo) {
        velocidadY += gravedad;
        velocidadY = qMin(velocidadY, 15.0);
    }
}

void Personaje::verificarColisionSuelo()
{
    if (!scene()) return;

    qreal posicionSuelo = scene()->sceneRect().bottom() - groundOffset;
    qreal personajeBottom = y() + boundingRect().height();

    if (personajeBottom >= posicionSuelo) {
        setY(posicionSuelo - boundingRect().height());
        velocidadY = 0;
        enSuelo = true;
    } else {
        enSuelo = false;
    }
}

Personaje::Estado Personaje::estadoActual() const
{
    return estado;
}

bool Personaje::estaEnSuelo() const
{
    return enSuelo;
}
