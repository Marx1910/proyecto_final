#ifndef GOKU_H
#define GOKU_H

#include "personaje.h"
#include "animaciones.h"
#include <QKeyEvent>

class Goku : public Personaje
{
    Q_OBJECT

public:
    explicit Goku(QGraphicsItem *parent = nullptr, QObject *sceneParent = nullptr);
    ~Goku() override;

    void saltar() override;
    void agacharse() override;
    void levantarse() override;
    void actualizar() override;
    void perderVida(int cantidad = 1);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

signals:
    void gokuDerrotado();

private:
    void cargarSprites();

    QPixmap spriteSheet;
    AnimacionSprite *animador;
    int vida = 3;


    static const int ANCHO_FRAME = 35;
    static const int ALTO_FRAME = 39;
    static const int COLUMNAS = 8;
    static const int FILAS = 11;

    static const int FILA_CORRER = 2;
    static const int FILA_SALTAR = 3;
    static const int FILA_AGACHADO = 4;
    static const int FILA_DANO = 4;
};

#endif // GOKU_H
