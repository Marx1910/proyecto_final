#ifndef MENSAJEFINAL_H
#define MENSAJEFINAL_H

#include <QGraphicsScene>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>

class MensajeFinal : public QObject {
    Q_OBJECT
public:
    MensajeFinal(QGraphicsScene* scene);
    void mostrar(const QString& texto, const QColor& color, const QString& textoBoton);
    void ocultar();

signals:
    void botonClicked();

private:
    QGraphicsScene* escena;
    QGraphicsTextItem* mensaje;
    QGraphicsProxyWidget* botonProxy;
};

#endif // MENSAJEFINAL_H
