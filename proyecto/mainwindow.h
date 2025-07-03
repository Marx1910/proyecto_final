#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class GameScene;
class QGraphicsView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onJugarButtonClicked();  // Slot conectado al botón

private:
    Ui::MainWindow *ui;
    GameScene *gameScene;
    QGraphicsView *gameView;
};

#endif // MAINWINDOW_H
