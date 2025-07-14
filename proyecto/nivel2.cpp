#include "nivel2.h"
#include "mensajefinal.h"
#include <QPixmap>
#include <QDebug>
#include <QPushButton>
#include <QFont>
Nivel2::Nivel2(QObject *parent)
    : NivelBase(parent),
    goku(nullptr),
    textoPuntaje(nullptr),
    mensajeFinal(nullptr),
    botonFinalProxy(nullptr),
    puntaje(0)
{
    configurarEscena();

    textoPuntaje = new QGraphicsTextItem("Puntaje: 0");
    textoPuntaje->setDefaultTextColor(Qt::white);
    textoPuntaje->setFont(QFont("Arial", 16));
    textoPuntaje->setPos(sceneRect().width() - 195, 10);
    textoPuntaje->setZValue(10);
    addItem(textoPuntaje);
    mensajeFinal = new MensajeFinal(this);
}

void Nivel2::configurarEscena()
{
    NivelBase::configurarEscena();
    // Fondo fijo
    imagenFondo = QPixmap(":/new/prefix1/recursos/fn2.png");
    if (imagenFondo.isNull()) {
        qDebug() << "No fn2.png, usando fondo por defecto";
        NivelBase::configurarEscena();
    } else {
        QSize s = sceneRect().size().toSize();
        QPixmap fijo = imagenFondo.scaled(s,
                                          Qt::IgnoreAspectRatio,
                                          Qt::SmoothTransformation);
        fondoItem = new QGraphicsPixmapItem(fijo);
        fondoItem->setZValue(-1);
        addItem(fondoItem);
    }

    // Crear Gokú y desbloquear controles
    goku = new Goku(nullptr, this);
    goku->desbloquearControlesAvanzados();
    goku->disableAutoRun();
    goku->setGroundOffset(195);

    // Posicionar con groundOffset ya configurado
    qreal h = goku->boundingRect().height();
    goku->setPos(50,
                 sceneRect().height() - h - goku->getGroundOffset());

    addItem(goku);
    goku->setFlag(QGraphicsItem::ItemIsFocusable);
    goku->setFocus();
    goku->agregarVidasAEscena(this);
    setSceneRect(0, 0, 800, 600);

    // … fondo, Gokú …
    goku->agregarVidasAEscena(this);

    // 1) spawnea Robot
    Robot* bot = new Robot(nullptr, this);
    bot->setGroundOffset(195);
    qreal spawnX = sceneRect().right() + bot->boundingRect().width();
    qreal spawnY = sceneRect().bottom()
                   - bot->boundingRect().height()
                   - bot->getGroundOffset();
    bot->setPos(spawnX, spawnY);
    addItem(bot);

    // 2) UI de vidas del Robot
    bot->agregarVidasAEscena(this);
    bot->actualizarVidasUI();

    // 3) IA
    RobotAI* ia = new RobotAI(bot, goku, this);
    ia->start();

    connect(bot,  &Robot::robotDerrotado,
            this, &Nivel2::onRobotDefeated);
    connect(goku, &Goku::gokuDerrotado,
            this, &Nivel2::onGokuDefeated);
    connect(this, &Nivel2::nivelFallido,
            this, &Nivel2::onRetryClicked);

}

void Nivel2::iniciarNivel()
{
    goku->setFocus();
}

void Nivel2::aumentarPuntaje(int puntos)
{
    puntaje += puntos;
    textoPuntaje->setPlainText("Puntaje: " + QString::number(puntaje));
}

void Nivel2::reiniciarNivel()
{
    // Limpiar mensaje final
    if (mensajeFinal) {
        mensajeFinal->ocultar();
    }
    if (botonFinalProxy) {
        QPushButton* btn = qobject_cast<QPushButton*>(botonFinalProxy->widget());
        if (btn) btn->disconnect();
        removeItem(botonFinalProxy);
        delete btn;
        botonFinalProxy = nullptr;
    }

    // Reset Goku y reposicionar con nuevo offset
    goku->vida = 3;
    goku->actualizarVidasUI();

    qreal h = goku->boundingRect().height();
    goku->setPos(50, sceneRect().height() - h - goku->getGroundOffset());

    // Reset puntaje
    puntaje = 0;
    textoPuntaje->setPlainText("Puntaje: 0");

    goku->setFocus();
}


void Nivel2::onRobotDefeated()
{
    // Mostrar “YOU WIN” sin botón
    mensajeFinal->mostrar("YOU WIN", Qt::yellow, QString());
    emit nivelCompletado();
}

void Nivel2::onGokuDefeated()
{
    // Mostrar “YOU LOSE” y botón “Reintentar”
    mensajeFinal->mostrar("YOU LOSE", Qt::red, "Reintentar");
    emit nivelFallido();
}

void Nivel2::onRetryClicked()
{
    reiniciarNivel();
}

