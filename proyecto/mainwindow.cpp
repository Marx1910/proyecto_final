#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nivel1.h"
#include <QGraphicsView>
#include <QDebug>
#include "goku.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    menuScene(new MenuScene(this)),
    nivelActual(nullptr),
    vistaActual(nullptr)
{
    ui->setupUi(this);


    vistaActual = new QGraphicsView(menuScene);
    vistaActual->setFixedSize(800, 600);
    vistaActual->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaActual->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaActual->setFocusPolicy(Qt::StrongFocus);
    vistaActual->setFrameStyle(QFrame::NoFrame);
    vistaActual->setFocus();

    setCentralWidget(vistaActual);


    connect(menuScene, &MenuScene::jugarClicked, this, &MainWindow::cambiarANivel1);
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::cambiarANivel1()
{
    if (vistaActual) {
        vistaActual->deleteLater();
    }

    nivelActual = new Nivel1(this);
    vistaActual = new QGraphicsView(nivelActual);
    vistaActual->setFixedSize(800, 600);
    vistaActual->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaActual->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaActual->setFrameStyle(QFrame::NoFrame);
    vistaActual->setFocusPolicy(Qt::StrongFocus);
    vistaActual->setFocusPolicy(Qt::StrongFocus);
    vistaActual->setFocus();

    setCentralWidget(vistaActual);

    QTimer::singleShot(0, this, [this]() {
        vistaActual->setFocus();

        QTimer::singleShot(0, this, [this]() {
            if (nivelActual->focusItem()) {
                nivelActual->focusItem()->setFocus();
                qDebug() << "Foco forzado a:" << nivelActual->focusItem();
            } else {
                qDebug() << " No hay focusItem ni siquiera despues de doble espera";
            }
        });
    });

    nivelActual->iniciarNivel();
}

