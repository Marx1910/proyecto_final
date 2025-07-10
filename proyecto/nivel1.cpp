
#include "nivel1.h"
#include <QPixmap>
#include <QDebug>
#include "goku.h"

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

    QSize sceneSize = sceneRect().size().toSize();

    QPixmap fondoEscalado = imagenFondo.scaled(
        sceneSize,
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation
        );

    fondoItem = new QGraphicsPixmapItem(fondoEscalado);
    fondoItem->setPos(0, 0);
    fondoItem->setZValue(-1);
    addItem(fondoItem);

    fondoSecundario = new QGraphicsPixmapItem(fondoEscalado);
    fondoSecundario->setPos(fondoEscalado.width(), 0);
    fondoSecundario->setZValue(-1);
    addItem(fondoSecundario);
    Goku* goku = new Goku();
    goku->setPixmap(goku->pixmap()); // Asegurar que el pixmap está cargado
    qreal gokuHeight = goku->boundingRect().height();
    goku->setPos(10, sceneRect().height() - 192 - gokuHeight);
    addItem(goku);
}


void Nivel1::iniciarAnimacionFondo()
{
    temporizadorScroll = new QTimer(this);
    connect(temporizadorScroll, &QTimer::timeout, this, [=]() {
        fondoItem->moveBy(-velocidadScroll, 0);
        fondoSecundario->moveBy(-velocidadScroll, 0);

        int anchoFondo = fondoItem->pixmap().width();
        if (fondoItem->x() <= -anchoFondo) {
            fondoItem->setPos(fondoSecundario->x() + anchoFondo, 0);
        }
        if (fondoSecundario->x() <= -anchoFondo) {
            fondoSecundario->setPos(fondoItem->x() + anchoFondo, 0);
        }
    });
    temporizadorScroll->start(30);
}

void Nivel1::redimensionarFondo()
{
    if (!fondoItem || !fondoSecundario || imagenFondo.isNull()) return;

    QSize sceneSize = sceneRect().size().toSize();
    QPixmap fondoEscalado = imagenFondo.scaled(
        sceneSize,
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation
        );

    fondoItem->setPixmap(fondoEscalado);
    fondoSecundario->setPixmap(fondoEscalado);

    if (fondoItem->x() < fondoSecundario->x()) {
        fondoItem->setPos(0, 0);
        fondoSecundario->setPos(fondoEscalado.width(), 0);
    } else {
        fondoSecundario->setPos(0, 0);
        fondoItem->setPos(fondoEscalado.width(), 0);
    }
}
