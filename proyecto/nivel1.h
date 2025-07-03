#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivelbase.h"
#include <QTimer>

class Nivel1 : public NivelBase
{
    Q_OBJECT

public:
    explicit Nivel1(QObject *parent = nullptr);
    void iniciarNivel() override;
    void redimensionarFondo();

private:
    void configurarEscena() override;
    void iniciarAnimacionFondo();

    // Elementos específicos del nivel 1
    QGraphicsPixmapItem *fondoSecundario; // Para el efecto de scroll infinito
    QTimer *temporizadorScroll;
    int velocidadScroll = 4; // píxeles por frame
};

#endif // NIVEL1_H
