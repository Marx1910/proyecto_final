#ifndef ANIMACIONSPRITE_H
#define ANIMACIONSPRITE_H

#include <QObject>
#include <QPixmap>
#include <QTimer>
#include <QGraphicsPixmapItem>

class AnimacionSprite : public QObject
{
    Q_OBJECT

public:
    AnimacionSprite(QGraphicsPixmapItem *target,
                    QPixmap spriteSheet,
                    int anchoFrame,
                    int altoFrame,
                    QObject *parent = nullptr);

    void configurarSecuencia(int fila, int inicio, int fin, int fps = 10);
    void reproducir();
    void detener();

private slots:
    void actualizarFrame();

private:
    QGraphicsPixmapItem *destino;
    QPixmap hoja;
    QTimer temporizador;

    int ancho, alto;
    int filaActual;
    int frameInicio, frameFin;
    int frameActual;
};

#endif // ANIMACIONSPRITE_H
