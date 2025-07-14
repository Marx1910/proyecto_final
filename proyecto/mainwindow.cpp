#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nivel1.h"
#include "nivel2.h"
#include <QGraphicsView>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    menuScene(new MenuScene(this)),
    nivelActual(nullptr),
    vistaActual(nullptr)
{
    ui->setupUi(this);

    // Mostrar menú al inicio
    vistaActual = new QGraphicsView(menuScene);
    vistaActual->setFixedSize(800, 600);
    vistaActual->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaActual->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaActual->setFrameStyle(QFrame::NoFrame);
    vistaActual->setFocusPolicy(Qt::StrongFocus);
    vistaActual->setFocus();
    setCentralWidget(vistaActual);

    // Conectar botón “Jugar” del menú
    connect(menuScene, &MenuScene::jugarClicked,
            this, &MainWindow::cambiarANivel1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Slot para nivel 1
void MainWindow::cambiarANivel1()
{
    // Destruir la vista anterior
    if (vistaActual) {
        vistaActual->deleteLater();
    }

    // Crear Nivel1
    auto *nivel1 = new Nivel1(this);
    nivelActual = nivel1;

    // Conectar señal de completar nivel 1
    connect(nivel1, &Nivel1::nivelCompletado,
            this, &MainWindow::cambiarANivel2);

    // Configurar vista
    vistaActual = new QGraphicsView(nivel1);
    vistaActual->setFixedSize(800, 600);
    vistaActual->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaActual->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaActual->setFrameStyle(QFrame::NoFrame);
    vistaActual->setFocusPolicy(Qt::StrongFocus);
    vistaActual->setFocus();
    setCentralWidget(vistaActual);

    // Forzar foco en dos pasos (por seguridad)
    QTimer::singleShot(0, this, [this]() {
        vistaActual->setFocus();
        QTimer::singleShot(0, this, [this]() {
            if (nivelActual->focusItem()) {
                nivelActual->focusItem()->setFocus();
                qDebug() << "Foco en Nivel1:" << nivelActual->focusItem();
            }
        });
    });

    // Iniciar lógica del nivel
    nivel1->iniciarNivel();
}

// Slot para nivel 2
void MainWindow::cambiarANivel2()
{
    // Borrar vista y nivel activo
    if (vistaActual) {
        vistaActual->deleteLater();
    }

    // Crear Nivel2
    auto *nivel2 = new Nivel2(this);
    nivelActual = nivel2;

    // Conectar aquí otras señales si hiciera falta, p.ej.
    // connect(nivel2, &Nivel2::nivelFallido, this, &MainWindow::reiniciarNivel2);

    vistaActual = new QGraphicsView(nivel2);
    vistaActual->setFixedSize(800, 600);
    vistaActual->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaActual->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vistaActual->setFrameStyle(QFrame::NoFrame);
    vistaActual->setFocusPolicy(Qt::StrongFocus);
    vistaActual->setFocus();
    setCentralWidget(vistaActual);

    QTimer::singleShot(0, this, [this]() {
        vistaActual->setFocus();
        QTimer::singleShot(0, this, [this]() {
            if (nivelActual->focusItem()) {
                nivelActual->focusItem()->setFocus();
                qDebug() << "Foco en Nivel2:" << nivelActual->focusItem();
            }
        });
    });

    // Arrancar la lógica de Nivel2
    nivel2->iniciarNivel();
}
