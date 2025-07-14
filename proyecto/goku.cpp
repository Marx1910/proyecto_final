// goku.cpp
#include "goku.h"
#include "kameha.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsOpacityEffect>

// Constructor
Goku::Goku(QGraphicsItem *parent, QObject *sceneParent)
    : Personaje(parent),
    animador(nullptr),
    currentAnim(Idle),
    controlesAvanzados(false),
    autoRunEnabled(true),
    movingLeft(false),
    movingRight(false),
    velocidadHorizontal(5)
{
    cargarSprites();
    playIdle();

    // Configurar barra de vidas
    vidaSprite = QPixmap(":/new/prefix1/recursos/objetos.png");
    if (vidaSprite.isNull()) {
        qWarning() << "No se cargó spritesheet de vidas";
        vidaSprite = QPixmap(90, 30);
        vidaSprite.fill(Qt::red);
    }
    for (int i = 0; i < 3; ++i) {
        vidaUI[i] = new QGraphicsPixmapItem;
        vidaUI[i]->setPixmap(
            vidaSprite.copy(i * VIDA_FRAME_W, 0,
                            VIDA_FRAME_W, VIDA_FRAME_H)
            );
        vidaUI[i]->setPos(20 + i * VIDA_FRAME_W, 20);
        vidaUI[i]->setZValue(1000);
    }

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

// Destructor
Goku::~Goku()
{
    if (animador) {
        animador->detener();
        delete animador;
    }
}

// Cargar spritesheet
void Goku::cargarSprites()
{
    spriteSheet = QPixmap(":/new/prefix1/recursos/goku.png");
    if (spriteSheet.isNull())
        qCritical() << "Error al cargar goku.png";
}

void Goku::desbloquearControlesAvanzados()
{
    controlesAvanzados = true;
}

void Goku::disableAutoRun()
{
    autoRunEnabled = false;
    if (animador) animador->detener();
    playIdle();
}

// Animaciones de Gokú
void Goku::playIdle()
{
    if (currentAnim == Idle) return;
    currentAnim = Idle;
    if (animador) animador->detener();

    QPixmap idle = spriteSheet.copy(
                                  0,
                                  FILA_IDLE  * ALTO_FRAME,
                                  ANCHO_FRAME,
                                  ALTO_FRAME
                                  ).scaled(
                           ANCHO_FRAME * 2,
                           ALTO_FRAME   * 2,
                           Qt::IgnoreAspectRatio,
                           Qt::SmoothTransformation
                           );
    setPixmap(idle);
    setTransformOriginPoint(idle.width()/2, idle.height());
}

void Goku::playRunRight(int fps)
{
    if (currentAnim == RunRight) return;
    currentAnim = RunRight;
    setTransform(QTransform());
    if (!animador)
        animador = new AnimacionSprite(this, spriteSheet, ANCHO_FRAME, ALTO_FRAME, nullptr);
    animador->configurarSecuencia(FILA_CORRER_DER, 0, 7, fps);
    animador->reproducir();
}

void Goku::playRunLeft(int fps)
{
    if (currentAnim == RunLeft) return;
    currentAnim = RunLeft;
    setTransform(QTransform().scale(-1, 1));
    if (!animador)
        animador = new AnimacionSprite(this, spriteSheet, ANCHO_FRAME, ALTO_FRAME, nullptr);
    animador->configurarSecuencia(FILA_CORRER_DER, 0, 7, fps);
    animador->reproducir();
}

void Goku::playJump()
{
    if (currentAnim == Jump || !animador) return;
    currentAnim = Jump;
    animador->configurarSecuencia(FILA_SALTAR, 0, 3, 10);
    animador->reproducir();
}

void Goku::playCrouch()
{
    if (currentAnim == Crouch || !animador) return;
    currentAnim = Crouch;
    setScale(0.7);
    animador->configurarSecuencia(FILA_AGACHADO, 0, 1, 15);
    animador->reproducir();
}

void Goku::playHurt()
{
    currentAnim = Hurt;
    if (!animador) return;
    animador->configurarSecuencia(FILA_DANO, 0, 2, 15);
    animador->reproducir();
}

// Acciones
void Goku::saltar()
{
    if (enSuelo && !agachado) {
        velocidadY = fuerzaSalto;
        enSuelo    = false;
        playJump();
    }
}

void Goku::agacharse()
{
    if (enSuelo && currentAnim != Crouch) {
        agachado = true;
        playCrouch();
    }
}

void Goku::levantarse()
{
    if (!agachado) return;
    agachado = false;
    setScale(1.0);
    if (autoRunEnabled || movingLeft || movingRight)
        playRunRight();
    else
        playIdle();
}

// Lógica por frame
void Goku::actualizar()
{
    aplicarGravedad();
    setY(y() + velocidadY);
    verificarColisionSuelo();

    if (controlesAvanzados) {
        QRectF rect = scene()->sceneRect();
        qreal minX = rect.left();
        qreal maxX = rect.left() + rect.width() - boundingRect().width();

        if (movingRight) {
            setX(qMin(maxX, x() + velocidadHorizontal));
            playRunRight();
        } else if (movingLeft) {
            setX(qMax(minX, x() - velocidadHorizontal));
            playRunLeft();
        } else if (!autoRunEnabled &&
                   (currentAnim == RunRight || currentAnim == RunLeft)) {
            playIdle();
        }
    }

    if (!enSuelo && currentAnim != Jump)
        playJump();
    else if (enSuelo && currentAnim == Jump) {
        if (movingLeft)        playRunLeft();
        else if (movingRight)  playRunRight();
        else if (autoRunEnabled) playRunRight();
        else                    playIdle();
    }

#ifdef QT_DEBUG
    if (controlesAvanzados) {
        QRectF r = scene()->sceneRect();
        qDebug() << "X:" << x()
                 << " minX:" << r.left()
                 << " maxX:" << (r.left() + r.width() - boundingRect().width());
    }
#endif
}

// Vida
void Goku::perderVida(int cantidad)
{
    vida -= cantidad;
    actualizarVidasUI();
    if (vida <= 0) emit gokuDerrotado();
    playHurt();
    setOpacity(0.5);
    QTimer::singleShot(100, this, [this]() { setOpacity(1.0); });
}

void Goku::actualizarVidasUI()
{
    for (int i = 0; i < 3; ++i) {
        if (!vidaUI[i]->graphicsEffect()) {
            vidaUI[i]->setGraphicsEffect(
                new QGraphicsOpacityEffect(this)
                );
        }
        auto *fx = qobject_cast<QGraphicsOpacityEffect*>(
            vidaUI[i]->graphicsEffect()
            );
        if (fx) fx->setOpacity(i < vida ? 1.0 : 0.1);
    }
}

void Goku::agregarVidasAEscena(QGraphicsScene* escena)
{
    for (int i = 0; i < 3; ++i)
        escena->addItem(vidaUI[i]);
}

// Input de teclado
void Goku::keyPressEvent(QKeyEvent* event)
{
    qDebug() << "GOKU KEY:" << event->key();
    switch (event->key()) {
    case Qt::Key_Space:
    case Qt::Key_W:
        saltar(); return;
    case Qt::Key_S:
        agacharse(); return;
    default:
        break;
    }

    if (controlesAvanzados) {
        switch (event->key()) {
        case Qt::Key_A:
        case Qt::Key_Left:
            movingLeft  = true;
            movingRight = false;
            return;
        case Qt::Key_D:
        case Qt::Key_Right:
            movingRight = true;
            movingLeft  = false;
            return;
        case Qt::Key_K:
            // Disparo inmediato de kamehameha
            lanzarKamehameha(movingRight ? +1 : -1);
            return;
        default:
            Personaje::keyPressEvent(event);
        }
    } else {
        Personaje::keyPressEvent(event);
    }
}

void Goku::keyReleaseEvent(QKeyEvent* event)
{
    qDebug() << "GOKU KEY:" << event->key();
    if (event->key() == Qt::Key_S) {
        levantarse();
        return;
    }
    if (controlesAvanzados) {
        if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
            movingLeft = false;
            return;
        }
        if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
            movingRight = false;
            return;
        }
    }
    QGraphicsItem::keyReleaseEvent(event);
}

// Disparo de proyectil
void Goku::lanzarKamehameha(int direction)
{
    // Crear el kamehameha y posicionarlo
    Kamehameha* bala = new Kamehameha(
        direction,
        this,     // owner
        scene(),
        this
        );
    qreal rx = (direction > 0)
                   ? x() + boundingRect().width()
                   : x() - bala->boundingRect().width();
    qreal ry = y() + boundingRect().height() * 0.5;
    bala->setPos(rx, ry);
}
