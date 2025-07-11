#include "nivel1.h"
#include <QPixmap>
#include <QDebug>
#include "goku.h"
#include "obstaculo.h"
#include <QRandomGenerator>

Nivel1::Nivel1(QObject *parent)
    : NivelBase(parent),
    goku(nullptr),
    temporizadorScroll(nullptr),
    temporizadorObstaculos(nullptr),
    temporizadorColisiones(nullptr),
    velocidadScroll(4),
    velocidadObstaculos(7),
    puntaje(0)
{
    configurarEscena();

    textoPuntaje = new QGraphicsTextItem();
    textoPuntaje->setPlainText("Puntaje: 0");
    textoPuntaje->setDefaultTextColor(Qt::white);
    textoPuntaje->setFont(QFont("Arial", 16));
    textoPuntaje->setPos(650, 10);
    addItem(textoPuntaje);
}
void Nivel1::iniciarNivel() {
    iniciarAnimacionFondo();

    // Iniciar generación de obstáculos
    temporizadorObstaculos = new QTimer(this);
    connect(temporizadorObstaculos, &QTimer::timeout, this, &Nivel1::generarObstaculo);
    temporizadorObstaculos->start(2000); // Cada 2 segundos

    // Iniciar detección de colisiones
    temporizadorColisiones = new QTimer(this);
    connect(temporizadorColisiones, &QTimer::timeout, this, &Nivel1::verificarColisiones);
    temporizadorColisiones->start(16); // ≈60 FPS

    qDebug() << "Nivel iniciado - Timers activados";
}
void Nivel1::configurarEscena() {
    // 1. Configurar fondo
    imagenFondo = QPixmap(":/new/prefix1/recursos/fn1.png");
    if (imagenFondo.isNull()) {
        qDebug() << "No se encontró fn1.png, usando fondo por defecto";
        NivelBase::configurarEscena();
    } else {
        QSize sceneSize = sceneRect().size().toSize();
        QPixmap fondoEscalado = imagenFondo.scaled(sceneSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        fondoItem = new QGraphicsPixmapItem(fondoEscalado);
        fondoItem->setPos(0, 0);
        fondoItem->setZValue(-1);
        addItem(fondoItem);

        fondoSecundario = new QGraphicsPixmapItem(fondoEscalado);
        fondoSecundario->setPos(fondoEscalado.width(), 0);
        fondoSecundario->setZValue(-1);
        addItem(fondoSecundario);
    }


    goku = new Goku(nullptr, this);
    qreal gokuHeight = goku->boundingRect().height();
    goku->setPos(50, sceneRect().height() - gokuHeight - 100);
    addItem(goku);
    goku->setFocus();
    goku->setFlag(QGraphicsItem::ItemIsFocusable);
    goku->agregarVidasAEscena(this);


    textoPuntaje = new QGraphicsTextItem();
    textoPuntaje->setPlainText("Puntaje:");
    textoPuntaje->setDefaultTextColor(Qt::white);
    textoPuntaje->setFont(QFont("Arial", 16));
    textoPuntaje->setPos(sceneRect().width() - 150, 10);
    textoPuntaje->setZValue(10);
    addItem(textoPuntaje);


    qDebug() << "Escena configurada - Goku en posición:" << goku->pos();
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


void Nivel1::generarObstaculo() {
    qDebug() << "Intentando generar obstáculo...";

    Obstaculo::Tipo tipo = (QRandomGenerator::global()->bounded(100) < 60)
                               ? Obstaculo::NAVE
                               : Obstaculo::PERRO;

    Obstaculo* obstaculo = new Obstaculo(tipo);
    obstaculo->setVelocidad(velocidadObstaculos);


    qreal yPos = (tipo == Obstaculo::NAVE)
                     ? sceneRect().height() * 0.62
                     : sceneRect().height() - 170;

    obstaculo->setPos(sceneRect().width(), yPos);
    addItem(obstaculo);

    qDebug() << "Obstáculo generado - Tipo:" << tipo
             << "Pos X:" << obstaculo->x()
             << "Y:" << obstaculo->y()
             << "Velocidad:" << obstaculo->getVelocidad();

    connect(obstaculo, &Obstaculo::destroyed, this, [this, tipo]() {
        aumentarPuntaje((tipo == Obstaculo::NAVE) ? 2 : 1);
    });
}


void Nivel1::verificarColisiones() {
    if (!goku) return;

    QList<QGraphicsItem*> items = this->items();
    foreach(QGraphicsItem *item, items) {
        if (Obstaculo* obstaculo = dynamic_cast<Obstaculo*>(item)) {
            if (goku->collidesWithItem(obstaculo, Qt::IntersectsItemShape)) {

                disconnect(obstaculo, &Obstaculo::destroyed, this, nullptr);


                int danio = (obstaculo->getTipo() == Obstaculo::NAVE) ? 2 : 1;
                goku->perderVida(danio);

                this->removeItem(obstaculo);
                obstaculo->deleteLater();
                break;
            }
        }
    }
}
void Nivel1::aumentarPuntaje(int puntos) {
    puntaje += puntos;


    if (textoPuntaje) {
        textoPuntaje->setPlainText("Puntaje: " + QString::number(puntaje));
    }

    // Aumentar dificultad cada 10 puntos
    if (puntaje > 0 && puntaje % 10 == 0) {
        velocidadObstaculos += 5;
        qDebug() << "Dificultad aumentada. Velocidad:" << velocidadObstaculos;


        int nuevoIntervalo = qMax(800, 2000 - (puntaje/10)*200);
        temporizadorObstaculos->setInterval(nuevoIntervalo);
    }
}
