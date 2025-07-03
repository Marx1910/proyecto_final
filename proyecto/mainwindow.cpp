#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nivel1.h"  // Incluye tu Nivel1
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    menuScene(new MenuScene(this)),
    nivelActual(nullptr),
    vistaActual(nullptr)
{
    ui->setupUi(this);

    // Configurar menú inicial
    vistaActual = new QGraphicsView(menuScene);
    vistaActual->setFixedSize(800, 600);
    vistaActual->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaActual->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(vistaActual);

    // Conectar señal del menú al slot
    connect(menuScene, &MenuScene::jugarClicked, this, &MainWindow::cambiarANivel1);
}

MainWindow::~MainWindow()
{
    delete ui;
    // No es necesario eliminar menuScene/nivelActual/vistaActual
    // porque son hijos de MainWindow (se destruyen automáticamente)
}

void MainWindow::cambiarANivel1()
{
    // Limpiar vista anterior (menú)
    if (vistaActual) {
        vistaActual->deleteLater();
    }

    // Crear nivel 1
    nivelActual = new Nivel1(this);  // Nivel1 hereda de NivelBase
    vistaActual = new QGraphicsView(nivelActual);

    // Configurar vista del nivel
    vistaActual->setFixedSize(800, 600);
    vistaActual->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaActual->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaActual->setFrameStyle(QFrame::NoFrame);

    setCentralWidget(vistaActual);
    nivelActual->iniciarNivel();  // Inicia la lógica del nivel
}
