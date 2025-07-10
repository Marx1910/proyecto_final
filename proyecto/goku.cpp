#include "goku.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>

Goku::Goku(QGraphicsItem *parent, QObject *sceneParent)
    : Personaje(parent), animador(nullptr)
{
    cargarSprites();
    animador = new AnimacionSprite(this, spriteSheet, ANCHO_FRAME, ALTO_FRAME, sceneParent);

    animador = new AnimacionSprite(this, spriteSheet, ANCHO_FRAME, ALTO_FRAME, this);
    animador->configurarSecuencia(FILA_CORRER, 0, 6, 30);
    animador->reproducir();

    setFlag(QGraphicsItem::ItemIsFocusable);

}


void Goku::actualizar()
{

    aplicarGravedad();
    setY(y() + velocidadY);
    verificarColisionSuelo();


    if (velocidadY > 0 && estado == Saltando) {
        estado = Cayendo;
    }

    if (enSuelo && estado == Cayendo) {
        estado = Corriendo;
        animador->configurarSecuencia(FILA_CORRER, 0, 7, 30);
        animador->reproducir();
    }
}

Goku::~Goku()
{

}

void Goku::cargarSprites()
{
    spriteSheet = QPixmap(":/new/prefix1/recursos/goku.png");
    if (spriteSheet.isNull()) {
        qCritical() << "Error: No se pudo cargar el sprite sheet de Goku";
    }
}

void Goku::saltar()
{
    if (enSuelo && !agachado) {
        velocidadY = fuerzaSalto;
        enSuelo = false;
        estado = Saltando;


        animador->configurarSecuencia(FILA_SALTAR, 0, 3, 10);
        animador->reproducir();

        qDebug() << "Saltando! Velocidad Y:" << velocidadY;
    }
}

void Goku::agacharse() {
    if (enSuelo && estado != Agachado) {
        estado = Agachado;
        agachado = true;


        setScale(0.7);

        animador->configurarSecuencia(FILA_AGACHADO, 0, 1, 15);
        animador->reproducir();
    }
}
void Goku::levantarse() {
    if (agachado) {
        agachado = false;
        estado = Corriendo;


        setScale(1.0);

        animador->configurarSecuencia(FILA_CORRER, 0, 7, 15);
        animador->reproducir();
    }
}

void Personaje::actualizar()
{
    // Sistema de movimiento mínimo
    static qreal testY = y();

    if(!enSuelo) {
        velocidadY += gravedad;
        testY += velocidadY;
    }

    // Detección simple de suelo
    if(testY >= scene()->sceneRect().bottom() - 105 - boundingRect().height()) {
        testY = scene()->sceneRect().bottom() - 105 - boundingRect().height();
        velocidadY = 0;
        enSuelo = true;
    }

    setY(testY);

    qDebug() << "Pos Y:" << y() << "TestY:" << testY;
}

void Goku::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Space:
    case Qt::Key_W:
        qDebug() << "Tecla de salto presionada";
        saltar();
        break;
    case Qt::Key_S:
        agacharse();
        break;
    default:
        QGraphicsPixmapItem::keyPressEvent(event);
    }
}

void Goku::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_S) {
        levantarse();
    } else {
        QGraphicsItem::keyReleaseEvent(event);
    }
}


void Goku::perderVida(int cantidad) {
    vida -= cantidad;

    qDebug() << "Goku perdió" << cantidad << "vida(s). Vidas restantes:" << vida;

    if (vida <= 0) {
        qDebug() << "¡Goku fue derrotado!";
        emit gokuDerrotado();
    }


    if (animador) {
        animador->configurarSecuencia(FILA_DANO, 0, 2, 15);
        animador->reproducir();
    }

    // Efecto de parpadeo
    setOpacity(0.5);
    QTimer::singleShot(100, this, [this]() { setOpacity(1.0); });
}
