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
    void jugarClicked(); // Señal cuando se presiona "Jugar"

private slots:
    void onJugarButtonClicked();

private:
    QGraphicsPixmapItem *fondoItem;
    QGraphicsProxyWidget *botonProxy; // Para añadir QPushButton a la escena
};

#endif // MENUSCENE_H
