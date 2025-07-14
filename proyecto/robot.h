#ifndef ROBOT_H
#define ROBOT_H

#include "personaje.h"
#include "animaciones.h"
#include <QTimer>
#include <QGraphicsPixmapItem>

class QGraphicsScene;

class Robot : public Personaje
{
    Q_OBJECT

public:
    enum RoboEstado {
        RIdle, RMovingLeft, RMovingRight,
        RJumping, RCrouch, RRayAttack, RPunch
    };

    explicit Robot(QGraphicsItem *parent = nullptr,
                   QObject      *sceneParent = nullptr);
    ~Robot() override;

    // IA-driven
    bool movingLeft;
    bool movingRight;

    void setTarget(QGraphicsItem *t) { target = t; }

    // Vidas
    void perderVida(int cantidad = 1);
    void actualizarVidasUI();
    void agregarVidasAEscena(QGraphicsScene* escena);

    // Acciones (públicas)
    void saltar() override;
    void agacharse() override;
    void levantarse() override;
    void lanzarRayo(int direction);
    void golpear();

    // Animaciones
    void playAnimation(RoboEstado estado);

    // Consulta
    bool isOnGround() const;

signals:
    void robotDerrotado();

protected:
    // Llamado cada frame por NivelBase
    void actualizar() override;

private:
    void cargarSprites();

    AnimacionSprite      *animador;
    QPixmap               spriteSheet;
    RoboEstado            estadoActual;
    QGraphicsItem        *target;
    int                   velocidadHorizontal;

    // Vidas
    int                   vida;
    QPixmap               vidaSprite;
    static const int      VIDA_FRAME_W = 23;
    static const int      VIDA_FRAME_H = 24;
    QGraphicsPixmapItem*  vidaUI[3];

    // Cooldowns (solo para ataques individuales)
    QTimer               *cooldownRayo;
    QTimer               *cooldownPunch;
};

#endif // ROBOT_H
