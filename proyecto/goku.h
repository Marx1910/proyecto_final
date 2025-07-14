#ifndef GOKU_H
#define GOKU_H

#include "personaje.h"
#include "animaciones.h"
#include "kameha.h"          // tu clase de proyectil
#include <QKeyEvent>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

class Goku : public Personaje
{
    Q_OBJECT

public:
    explicit Goku(QGraphicsItem *parent = nullptr,
                  QObject    *sceneParent = nullptr);
    ~Goku() override;

    void saltar() override;
    void agacharse() override;
    void levantarse() override;
    void actualizar() override;
    void perderVida(int cantidad = 1);
    void actualizarVidasUI();
    void agregarVidasAEscena(QGraphicsScene* escena);
    void desbloquearControlesAvanzados();
    void disableAutoRun();
    int vida = 3;

protected:
    void keyPressEvent(QKeyEvent  *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    enum CurrentAnim { Idle, RunRight, RunLeft, Jump, Crouch, Hurt };
    CurrentAnim        currentAnim;
    bool               controlesAvanzados;
    bool               autoRunEnabled;
    bool               movingLeft;
    bool               movingRight;
    int                velocidadHorizontal;
    AnimacionSprite   *animador;
    QPixmap            spriteSheet;

    static const int   ANCHO_FRAME     = 35;
    static const int   ALTO_FRAME      = 39;
    static const int   FILA_IDLE       = 0;
    static const int   FILA_CORRER_DER = 2;
    static const int   FILA_CORRER_IZQ = 5;
    static const int   FILA_SALTAR     = 3;
    static const int   FILA_AGACHADO   = 4;
    static const int   FILA_DANO       = 4;

    void cargarSprites();
    void lanzarKamehameha(int direction = +1);

    // Animaciones helper
    void playIdle();
    void playRunRight(int fps = 15);
    void playRunLeft (int fps = 15);
    void playJump();
    void playCrouch();
    void playHurt();

    // Vida

    QPixmap            vidaSprite;
    static const int   VIDA_FRAME_W    = 23;
    static const int   VIDA_FRAME_H    = 24;
    QGraphicsPixmapItem* vidaUI[3];

signals:
    void gokuDerrotado();
};

#endif // GOKU_H
