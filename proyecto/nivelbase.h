#ifndef NIVELBASE_H
#define NIVELBASE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class NivelBase : public QGraphicsScene {
    Q_OBJECT

public:
    explicit NivelBase(QObject *parent = nullptr);
    virtual ~NivelBase() = default;

    // Métodos obligatorios para todos los niveles
    virtual void iniciarNivel() = 0;
    virtual void pausarNivel(bool pausado);

    // Métodos comunes
    void redimensionarFondo();

protected:
    // Elementos protegidos para que hereden los niveles
    QGraphicsPixmapItem *fondoItem;
    QPixmap imagenFondo;

    virtual void configurarEscena();
};

#endif // NIVELBASE_H
