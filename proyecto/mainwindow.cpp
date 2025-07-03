#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamescene.h"

#include <QGraphicsView>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    gameScene(nullptr),
    gameView(nullptr)
{
    ui->setupUi(this);
    QPixmap fondo(":/new/prefix1/recursos/fondo.png"); //definir ruta de archivos para cargar el fondo.
    ui->backgroundLabel->setPixmap(fondo.scaled(size(), Qt::KeepAspectRatioByExpanding));

    QPixmap logo (":/new/prefix1/recursos/logo.png"); //definir ruta para el logo :)
    ui->logoLabel->setPixmap(logo.scaled(300, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->logoLabel->setAlignment(Qt::AlignCenter);

    connect(ui->jugarButton, &QPushButton::clicked, this, &MainWindow::onJugarButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onJugarButtonClicked()
{
    gameScene = new GameScene(this);
    gameView = new QGraphicsView(gameScene);
    gameView->setFixedSize(800, 600);
    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setFrameStyle(QFrame::NoFrame);

    setCentralWidget(gameView);

    gameScene->setSceneRect(0, 0, 800, 600);
    gameScene->resizeBackground();
}
