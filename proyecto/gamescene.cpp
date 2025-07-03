#include "gamescene.h"
#include <QPixmap>
#include <QDebug>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent),
    backgroundItem(nullptr)
{
    setSceneRect(0, 0, 800, 600);
    setupScene();
}

void GameScene::setupScene()
{
    // Cargar fondo solo una vez
    backgroundPixmap = QPixmap(":/new/prefix1/recursos/fondo.png");
    if (backgroundPixmap.isNull()) {
        backgroundPixmap = QPixmap(800, 600);
        backgroundPixmap.fill(Qt::darkBlue);
        qDebug() << "No se encontró fondo.png, usando fondo azul.";
    }

    backgroundItem = new QGraphicsPixmapItem(
        backgroundPixmap.scaled(sceneRect().size().toSize(),
                                Qt::IgnoreAspectRatio,
                                Qt::SmoothTransformation));
    backgroundItem->setZValue(-1);
    addItem(backgroundItem);
}

void GameScene::resizeBackground()
{
    if (!backgroundItem || backgroundPixmap.isNull()) return;

    QPixmap scaled = backgroundPixmap.scaled(sceneRect().size().toSize(),
                                             Qt::IgnoreAspectRatio,
                                             Qt::SmoothTransformation);
    backgroundItem->setPixmap(scaled);
}
