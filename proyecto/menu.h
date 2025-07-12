#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPushButton>

class MenuScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit MenuScene(QObject *parent = nullptr);
    void cargarInterfaz();

signals:
    void jugarClicked();

private slots:
    void onJugarButtonClicked();

private:
    QGraphicsPixmapItem *fondoItem;
    QGraphicsProxyWidget *botonProxy;
};

#endif // MENUSCENE_H
