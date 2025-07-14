#include "robot.h"
#include "kameha.h"
#include <QGraphicsScene>
#include <QDebug>

static const int ANCHO_R_F = 85;
static const int ALTO_R_F   = 82;
static const int RF_IDLE   = 3;
static const int RF_RUN    = 0;
static const int RF_JUMP   = 1;
static const int RF_CROUCH = 4;
static const int RF_RAY    = 0;
static const int RF_PUNCH  = 2;

Robot::Robot(QGraphicsItem *parent, QObject *sceneParent)
    : Personaje(parent),
    animador(nullptr),
    estadoActual(RIdle),
    target(nullptr),
    velocidadHorizontal(3),
    vida(3),
    movingLeft(false),
    movingRight(false)
{
    cargarSprites();
    animador = new AnimacionSprite(this,
                                   spriteSheet,
                                   ANCHO_R_F, ALTO_R_F,
                                   sceneParent);
    playAnimation(RIdle);

    // Vidas UI
    vidaSprite = QPixmap(":/new/prefix1/recursos/objetos.png");
    for (int i = 0; i < 3; ++i) {
        vidaUI[i] = new QGraphicsPixmapItem(nullptr);
        vidaUI[i]->setPixmap(
            vidaSprite.copy(i * VIDA_FRAME_W, 0,
                            VIDA_FRAME_W, VIDA_FRAME_H)
            );
        vidaUI[i]->setZValue(1000);
    }
    setScale(0.5);
    actualizarVidasUI();

    // Cooldowns
    cooldownRayo  = new QTimer(this);
    cooldownPunch = new QTimer(this);
    cooldownRayo->setInterval(1500);
    cooldownPunch->setInterval(1000);
    cooldownRayo->setSingleShot(true);
    cooldownPunch->setSingleShot(true);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

Robot::~Robot()
{
    delete animador;
}

void Robot::cargarSprites()
{
    spriteSheet = QPixmap(":/new/prefix1/recursos/robot.png");
    if (spriteSheet.isNull())
        qCritical() << "Error al cargar robot.png";
}

void Robot::playAnimation(RoboEstado nuevo)
{
    if (estadoActual == nuevo) return;
    estadoActual = nuevo;

    switch (nuevo) {
    case RIdle:
        animador->configurarSecuencia(RF_IDLE,   0, 5,  8);
        break;
    case RMovingLeft:
    case RMovingRight:
        animador->configurarSecuencia(RF_RUN,    6, 8, 12);
        break;
    case RJumping:
        animador->configurarSecuencia(RF_JUMP,   0, 7, 10);
        break;
    case RCrouch:
        animador->configurarSecuencia(RF_CROUCH, 0, 2, 10);
        break;
    case RRayAttack:
        animador->configurarSecuencia(RF_RAY,    0, 4, 14);
        break;
    case RPunch:
        animador->configurarSecuencia(RF_PUNCH,  0, 3, 14);
        break;
    }
    animador->reproducir();
}

bool Robot::isOnGround() const
{
    return enSuelo;
}

void Robot::perderVida(int cantidad)
{
    vida = qMax(0, vida - cantidad);
    actualizarVidasUI();
    if (vida == 0) emit robotDerrotado();
}

void Robot::agacharse()
{
    if (enSuelo) playAnimation(RCrouch);
}

void Robot::levantarse()
{
    if (estadoActual == RCrouch) playAnimation(RIdle);
}

void Robot::actualizarVidasUI()
{
    if (!scene()) return;
    qreal W = scene()->sceneRect().width();
    const int m = 20;

    for (int i = 0; i < 3; ++i) {
        vidaUI[i]->setOpacity(i < vida ? 1.0 : 0.1);
        qreal x = W - m - (i+1)*VIDA_FRAME_W;
        vidaUI[i]->setPos(x, 20);
    }
}

void Robot::agregarVidasAEscena(QGraphicsScene* escena)
{
    for (int i = 0; i < 3; ++i)
        escena->addItem(vidaUI[i]);
}

void Robot::saltar()
{
    if (enSuelo) {
        velocidadY = fuerzaSalto;
        enSuelo    = false;
        playAnimation(RJumping);
    }
}

void Robot::lanzarRayo(int direction)
{
    if (cooldownRayo->isActive()) return;
    playAnimation(RRayAttack);

    // 1) Define cuántos rayos por ataque
    const int numRayos = 5;

    // 2) Distancia entre cada rayo
    const qreal spacing = boundingRect().width() * 0.4;

    // 3) Bucle para crear cada rayo
    for (int i = 0; i < numRayos; ++i) {
        auto* r = new Kamehameha(direction, this, scene(), this);

        // Calcula un offset progresivo lateral
        qreal offset = spacing * i;
        qreal rx = (direction > 0)
                       ? x() + boundingRect().width() + offset
                       : x() - r->boundingRect().width() - offset;

        qreal ry = y() + boundingRect().height() * 0.5;
        r->setPos(rx, ry);
    }

    cooldownRayo->start();
    QTimer::singleShot(600, this, [this]() {
        playAnimation(RIdle);
    });
}


void Robot::golpear()
{
    if (cooldownPunch->isActive()) return;
    playAnimation(RPunch);

    QTimer::singleShot(400, this, [this]() {
        playAnimation(RIdle);
    });

    cooldownPunch->start();
}

void Robot::actualizar()
{
    // Física vertical
    aplicarGravedad();
    setY(y() + velocidadY);
    verificarColisionSuelo();

    // Movimiento horizontal
    QRectF rc = scene()->sceneRect();
    qreal minX = rc.left();
    qreal maxX = rc.right() - boundingRect().width();

    if (movingRight) {
        setX(qMin(maxX, x() + velocidadHorizontal));
        playAnimation(RMovingRight);
    }
    else if (movingLeft) {
        setX(qMax(minX, x() - velocidadHorizontal));
        playAnimation(RMovingLeft);
    }
    else if (estadoActual == RMovingLeft || estadoActual == RMovingRight) {
        playAnimation(RIdle);
    }

    // Aterrizaje tras salto
    if (enSuelo && estadoActual == RJumping)
        playAnimation(RIdle);
}
