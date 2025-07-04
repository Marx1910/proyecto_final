#include "nivel1.h"
#include <QPixmap>
#include <QDebug>

Nivel1::Nivel1(QObject *parent)
    : NivelBase(parent), fondoSecundario(nullptr), temporizadorScroll(nullptr)
{
    configurarEscena();
}

void Nivel1::iniciarNivel()
{
    iniciarAnimacionFondo();
}

void Nivel1::configurarEscena()
{
    imagenFondo = QPixmap(":/new/prefix1/recursos/fn1.png");
    if (imagenFondo.isNull()) {
        qDebug() << "No se encontró fn1.png, usando fondo por defecto";
        NivelBase::configurarEscena();
        return;
    }

    // Escalar al tamaño de la escena (800x600)
    QPixmap fondoEscalado = imagenFondo.scaled(sceneRect().width(), sceneRect().height(),
                                               Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // Primer fondo
    fondoItem = new QGraphicsPixmapItem(fondoEscalado);
    fondoItem->setPos(0, 0);
    fondoItem->setZValue(-1);
    addItem(fondoItem);

    // Segundo fondo (colocado justo a la derecha del primero)
    fondoSecundario = new QGraphicsPixmapItem(fondoEscalado);
    fondoSecundario->setPos(fondoEscalado.width(), 0); // Posición inicial: derecha del primer fondo
    fondoSecundario->setZValue(-1);
    addItem(fondoSecundario);
}

void Nivel1::iniciarAnimacionFondo()
{
    temporizadorScroll = new QTimer(this);
    connect(temporizadorScroll, &QTimer::timeout, this, [=]() {
        // Mover ambos fondos
        fondoItem->moveBy(-velocidadScroll, 0);
        fondoSecundario->moveBy(-velocidadScroll, 0);

        // Ancho del fondo escalado
        int anchoFondo = fondoItem->pixmap().width();

        // Reiniciar posición cuando un fondo sale completamente de la vista
        if (fondoItem->x() <= -anchoFondo) {
            fondoItem->setPos(fondoSecundario->x() + anchoFondo, 0);
        }
        if (fondoSecundario->x() <= -anchoFondo) {
            fondoSecundario->setPos(fondoItem->x() + anchoFondo, 0);
        }
    });

    temporizadorScroll->start(30); // ~33 FPS
}

void Nivel1::redimensionarFondo()
{
    if (!fondoItem || !fondoSecundario || imagenFondo.isNull()) return;

    QPixmap fondoEscalado = imagenFondo.scaled(sceneRect().width(), sceneRect().height(),
                                               Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    fondoItem->setPixmap(fondoEscalado);
    fondoSecundario->setPixmap(fondoEscalado);

    // Mantener posiciones relativas
    if (fondoItem->x() < fondoSecundario->x()) {
        fondoItem->setPos(0, 0);
        fondoSecundario->setPos(fondoEscalado.width(), 0);
    } else {
        fondoSecundario->setPos(0, 0);
        fondoItem->setPos(fondoEscalado.width(), 0);
    }
}
