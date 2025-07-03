#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>

class GameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GameScene(QObject *parent = nullptr);
    void resizeBackground();

private:
    void setupScene();

    QGraphicsPixmapItem *backgroundItem;
    QPixmap backgroundPixmap;
};

#endif // GAMESCENE_H
