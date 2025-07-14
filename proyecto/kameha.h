#ifndef KAMEHAMEHA_H
#define KAMEHAMEHA_H

#include <QObject>
#include <QTimer>
#include <QGraphicsPixmapItem>

class Personaje;
class QGraphicsScene;

class Kamehameha : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    // direction = +1 o -1, owner = quien dispara (Robot* o Goku*)
    explicit Kamehameha(int direction,
                        Personaje* owner,
                        QGraphicsScene* scene,
                        QObject* parent = nullptr);
    ~Kamehameha() override;

private slots:
    void mover();

private:
    int          direction;
    Personaje*   owner;
    QTimer*      timer;
    static const int speed = 12;  // px por tick
};

#endif // KAMEHAMEHA_H
