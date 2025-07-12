#include "mensajefinal.h"

MensajeFinal::MensajeFinal(QGraphicsScene* scene) : escena(scene), mensaje(nullptr), botonProxy(nullptr) {}

void MensajeFinal::mostrar(const QString& texto, const QColor& color, const QString& textoBoton) {
    // Limpiar primero si ya existe
    ocultar();

    // Crear mensaje
    mensaje = new QGraphicsTextItem(texto);
    mensaje->setDefaultTextColor(color);
    mensaje->setFont(QFont("Arial", 32, QFont::Bold));

    QRectF rect = mensaje->boundingRect();
    mensaje->setPos(escena->width()/2 - rect.width()/2, escena->height()/2 - rect.height()/2);
    escena->addItem(mensaje);

    // Crear botón
    QPushButton* boton = new QPushButton(textoBoton);
    boton->setStyleSheet(QString("QPushButton { font-size: 20px; padding: 10px; color: %1; }").arg(color.name()));

    botonProxy = escena->addWidget(boton);
    botonProxy->setPos(escena->width()/2 - 100, escena->height()/2 + 50);

    connect(boton, &QPushButton::clicked, this, &MensajeFinal::botonClicked);
}

void MensajeFinal::ocultar() {
    if (mensaje) {
        escena->removeItem(mensaje);
        delete mensaje;
        mensaje = nullptr;
    }
    if (botonProxy) {
        escena->removeItem(botonProxy);
        delete botonProxy;
        botonProxy = nullptr;
    }
}
