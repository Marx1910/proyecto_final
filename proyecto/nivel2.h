#ifndef NIVEL2_H
#define NIVEL2_H

#include "nivelbase.h"
#include "goku.h"
#include "robot.h"
#include "robotai.h"
#include "mensajefinal.h"
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>

class Nivel2 : public NivelBase
{
    Q_OBJECT

public:
    explicit Nivel2(QObject *parent = nullptr);
    void iniciarNivel() override;
    void configurarEscena() override;
    void aumentarPuntaje(int puntos);

signals:
    void nivelCompletado();
    void nivelFallido();

private slots:
    void reiniciarNivel();
    void onRobotDefeated();
    void onGokuDefeated();
    void onRetryClicked();

private:
    Goku* goku;
    QGraphicsTextItem* textoPuntaje;
    MensajeFinal*      mensajeFinal;
    QGraphicsProxyWidget* botonFinalProxy;
    int puntaje;
};

#endif // NIVEL2_H
