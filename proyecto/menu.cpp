#include "menu.h"
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QDebug>
#include <QRandomGenerator>

MenuScene::MenuScene(QObject *parent)
    : QGraphicsScene(parent), fondoItem(nullptr), botonProxy(nullptr) {
    setSceneRect(0, 0, 800, 600);
    cargarInterfaz();
}

void MenuScene::cargarInterfaz() {
    // 1. Cargar fondo
    QPixmap fondo(":/new/prefix1/recursos/fondo.png");
    if (fondo.isNull()) {
        fondo = QPixmap(800, 600);
        fondo.fill(Qt::darkGray);
        qDebug() << "Fondo del menú no encontrado, usando color por defecto";
    }
    fondoItem = new QGraphicsPixmapItem(fondo.scaled(sceneRect().size().toSize()));
    addItem(fondoItem);

    // 2. Cargar logo
    QPixmap logo(":/new/prefix1/recursos/logo.png");
    if (!logo.isNull()) {
        QGraphicsPixmapItem *logoItem = new QGraphicsPixmapItem(logo.scaled(300, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        logoItem->setPos(255, 90);
        addItem(logoItem);
    } else {
        qDebug() << "Logo no encontrado";
    }


    QPushButton *jugarButton = new QPushButton("Jugar");
    jugarButton->setStyleSheet("QPushButton { font-size: 16px; padding: 10px; }");
    botonProxy = addWidget(jugarButton);
    botonProxy->setPos(360, 300);

    connect(jugarButton, &QPushButton::clicked, this, &MenuScene::onJugarButtonClicked);
}

void MenuScene::onJugarButtonClicked() {
    emit jugarClicked();
}
