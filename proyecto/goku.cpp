#include "goku.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsOpacityEffect>

Goku::Goku(QGraphicsItem *parent, QObject *sceneParent)
    : Personaje(parent), animador(nullptr)
{
    // 1. Cargar spritesheet principal
    cargarSprites();

    // 2. Configurar animación (usando sceneParent como parent del animador)
    animador = new AnimacionSprite(this, spriteSheet, ANCHO_FRAME, ALTO_FRAME, sceneParent);
    animador->configurarSecuencia(FILA_CORRER, 0, 7, 30);
    animador->reproducir();

    // 3. Cargar spritesheet de vidas
    vidaSprite = QPixmap(":/new/prefix1/recursos/objetos.png");
    if(vidaSprite.isNull()) {
        qDebug() << "Error: No se cargó el spritesheet de vidas";

        vidaSprite = QPixmap(90, 30);
        vidaSprite.fill(Qt::red);
    }

    // 4. Configurar indicadores de vida
    for (int i = 0; i < 3; ++i) {
        vidaUI[i] = new QGraphicsPixmapItem(nullptr);

        // Frame inicial (vida llena) - columna i, fila 0
        vidaUI[i]->setPixmap(vidaSprite.copy(
            i * VIDA_FRAME_W, 0,
            VIDA_FRAME_W,
            VIDA_FRAME_H
            ));

        vidaUI[i]->setPos(20 + i * (VIDA_FRAME_W ), 20);
        vidaUI[i]->setZValue(1000);

        vidaUI[i]->setToolTip(QString("Vida %1").arg(i+1));
    }

    // 5. Configuración inicial
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    qDebug() << "Goku inicializado. Spritesheet vidas cargado:"
             << vidaSprite.isNull();
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




void Goku::actualizarVidasUI() {
    for (int i = 0; i < 3; ++i) {

        if (!vidaUI[i]->graphicsEffect()) {
            vidaUI[i]->setGraphicsEffect(new QGraphicsOpacityEffect(this));
        }

        QGraphicsOpacityEffect* effect = qobject_cast<QGraphicsOpacityEffect*>(vidaUI[i]->graphicsEffect());
        if (effect) {
            effect->setOpacity(i < vida ? 1.0 : 0.1);  // 0.3 para vidas perdidas
        }

        // Animación solo para la vida recién perdida
        if (i == vida && vida < 3) {  // Si acaba de perder una vida
            QPropertyAnimation* anim = new QPropertyAnimation(vidaUI[i]->graphicsEffect(), "opacity");
            anim->setDuration(500);
            anim->setStartValue(1.0);
            anim->setEndValue(0.3);
            anim->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}


void Goku::perderVida(int cantidad) {
    vida -= cantidad;
    actualizarVidasUI();

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


void Goku::agregarVidasAEscena(QGraphicsScene* escena) {
    for (int i = 0; i < 3; ++i) {
        escena->addItem(vidaUI[i]);
    }
}
