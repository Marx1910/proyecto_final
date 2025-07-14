#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivelbase.h"
#include "goku.h"
#include "obstaculo.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>

class Nivel1 : public NivelBase
{
    Q_OBJECT

public:
    explicit Nivel1(QObject *parent = nullptr);
    void iniciarNivel() override;
    void configurarEscena() override;
    void aumentarPuntaje(int puntos);
    void generarObstaculo();
    void verificarColisiones();
    void redimensionarFondo();
    void setGroundOffset(int off) { groundOffset = off; }

signals:
    void nivelCompletado();
    void nivelFallido();

private slots:
    void reiniciarNivel();

protected:
    int groundOffset = 107;

private:
    void iniciarAnimacionFondo();
    void mostrarMensajeFinal(const QString& texto, const QColor& color, const QString& textoBoton);
    void limpiarMensajeFinal();

    // Elementos del juego
    Goku* goku;
    QGraphicsPixmapItem* fondoSecundario;
    QGraphicsTextItem* textoPuntaje;

    // Mensaje final
    QGraphicsTextItem* mensajeFinal;
    QGraphicsProxyWidget* botonFinalProxy;

    // Temporizadores
    QTimer* temporizadorScroll;
    QTimer* temporizadorObstaculos;
    QTimer* temporizadorColisiones;

    // Variables de control
    int velocidadScroll;
    int velocidadObstaculos;
    int puntaje;
};

#endif // NIVEL1_H
