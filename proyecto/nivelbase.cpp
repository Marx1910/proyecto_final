#include "nivelbase.h"
#include <QDebug>

NivelBase::NivelBase(QObject *parent)
    : QGraphicsScene(parent), fondoItem(nullptr) {
    setSceneRect(0, 0, 800, 600); // Tamaño fijo para todos los niveles
}

void NivelBase::configurarEscena() {
    if (imagenFondo.isNull()) {
        imagenFondo = QPixmap(800, 600);
        imagenFondo.fill(Qt::darkBlue);
        qDebug() << "Usando fondo por defecto en NivelBase";
    }

    fondoItem = new QGraphicsPixmapItem(imagenFondo.scaled(sceneRect().size().toSize(),
                                                           Qt::IgnoreAspectRatio,
                                                           Qt::SmoothTransformation));
    fondoItem->setZValue(-1); // Fondo detrás de todo
    addItem(fondoItem);
}

void NivelBase::redimensionarFondo() {
    if (!fondoItem || imagenFondo.isNull()) return;
    fondoItem->setPixmap(imagenFondo.scaled(sceneRect().size().toSize(),
                                            Qt::IgnoreAspectRatio,
                                            Qt::SmoothTransformation));
}

void NivelBase::pausarNivel(bool pausado) {
    // Lógica común para pausar (ej: detener timers)
    qDebug() << "Nivel" << (pausado ? "pausado" : "reanudado");
}
