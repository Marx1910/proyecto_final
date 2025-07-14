#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "menu.h"
#include "nivelbase.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void cambiarANivel1();  // Slot para transición al nivel 1
    void cambiarANivel2();

private:
    Ui::MainWindow *ui;
    MenuScene *menuScene;   // Escena del menú principal
    NivelBase *nivelActual; // Puntero al nivel actual (Nivel1, Nivel2...)
    QGraphicsView *vistaActual; // Vista activa (menú o nivel)
};

#endif // MAINWINDOW_H
