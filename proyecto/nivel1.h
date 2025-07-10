#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivelbase.h"
#include "goku.h"
#include "obstaculo.h"
#include <QTimer>
#include <QGraphicsTextItem>

class Nivel1 : public NivelBase
{
    Q_OBJECT

public:
    explicit Nivel1(QObject *parent = nullptr);

    // Métodos principales
    void iniciarNivel() override;
    void configurarEscena() override;
    void aumentarPuntaje(int puntos);

    // Funciones de gestión de obstáculos
    void generarObstaculo();
    void verificarColisiones();

    // Utilería
    void redimensionarFondo();

private:
    void iniciarAnimacionFondo();

    // Elementos del juego
    Goku* goku;                  // Personaje principal
    QGraphicsPixmapItem* fondoSecundario; // Para scroll infinito
    QGraphicsTextItem* textoPuntaje;      // Marcador de puntos

    // Temporizadores
    QTimer* temporizadorScroll;      // Para animación de fondo
    QTimer* temporizadorObstaculos;  // Para generación de obstáculos
    QTimer* temporizadorColisiones;  // Para detección de colisiones

    // Variables de control
    int velocidadScroll = 4;       // Velocidad base del fondo
    int velocidadObstaculos = 7;   // Velocidad inicial de obstáculos
    int puntaje = 0;               // Puntaje actual

    // Constantes
    static const int INTERVALO_OBSTACULOS = 2000; // 2 segundos
    static const int INTERVALO_COLISIONES = 16;   // ≈60 FPS
};

#endif // NIVEL1_H
