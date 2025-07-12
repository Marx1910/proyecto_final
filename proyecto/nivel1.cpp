#include "nivel1.h"
#include <QPixmap>
#include <QDebug>
#include <QPushButton>
#include <QRandomGenerator>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

Nivel1::Nivel1(QObject *parent)
    : NivelBase(parent), goku(nullptr), fondoSecundario(nullptr),
    textoPuntaje(nullptr), mensajeFinal(nullptr), botonFinalProxy(nullptr),
    temporizadorScroll(nullptr), temporizadorObstaculos(nullptr),
    temporizadorColisiones(nullptr), velocidadScroll(4),
    velocidadObstaculos(7), puntaje(0)
{
    configurarEscena();

    textoPuntaje = new QGraphicsTextItem();
    textoPuntaje->setPlainText("Puntaje: 0");
    textoPuntaje->setDefaultTextColor(Qt::white);
    textoPuntaje->setFont(QFont("Arial", 16));
    textoPuntaje->setPos(650, 10);
    addItem(textoPuntaje);
}

void Nivel1::mostrarMensajeFinal(const QString& texto, const QColor& color, const QString& textoBoton) {
    limpiarMensajeFinal();

    // Pausar el juego
    temporizadorScroll->stop();
    temporizadorObstaculos->stop();
    temporizadorColisiones->stop();

    // Crear mensaje
    mensajeFinal = new QGraphicsTextItem(texto);
    mensajeFinal->setDefaultTextColor(color);
    mensajeFinal->setFont(QFont("Arial", 32, QFont::Bold));
    QRectF rect = mensajeFinal->boundingRect();
    mensajeFinal->setPos(sceneRect().width()/2 - rect.width()/2,
                         sceneRect().height()/2 - rect.height()/2);
    addItem(mensajeFinal);

    // Crear botón
    QPushButton* boton = new QPushButton(textoBoton);
    boton->setStyleSheet(QString("QPushButton { "
                                 "font-size: 20px; "
                                 "padding: 10px 20px; "
                                 "background-color: white; "
                                 "border: 2px solid %1; "
                                 "color: %1; "
                                 "border-radius: 5px;"
                                 "}").arg(color.name()));

    botonFinalProxy = addWidget(boton);
    botonFinalProxy->setPos(sceneRect().width()/2 - 100, sceneRect().height()/2 + 50);

    // Conectar botón
    if (texto == "You Win!") {
        connect(boton, &QPushButton::clicked, this, &Nivel1::nivelCompletado);
    } else {
        connect(boton, &QPushButton::clicked, this, &Nivel1::reiniciarNivel);
    }
}

void Nivel1::limpiarMensajeFinal() {
    if (mensajeFinal) {
        removeItem(mensajeFinal);
        delete mensajeFinal;
        mensajeFinal = nullptr;
    }

    if (botonFinalProxy) {
        // Desconectar todas las señales del botón primero
        QPushButton* boton = qobject_cast<QPushButton*>(botonFinalProxy->widget());
        if (boton) {
            boton->disconnect();
        }

        removeItem(botonFinalProxy);
        delete botonFinalProxy->widget(); // Eliminar el widget primero
        botonFinalProxy = nullptr;
    }
}

void Nivel1::reiniciarNivel() {
    // Primero limpiar el mensaje existente
    limpiarMensajeFinal();

    // Resetear Goku
    goku->vida = 3;
    goku->actualizarVidasUI();
    goku->setPos(50, sceneRect().height() - goku->boundingRect().height() - 100);

    // Resetear juego
    puntaje = 0;
    textoPuntaje->setPlainText("Puntaje: ");
    velocidadObstaculos = 7;

    // Limpiar obstáculos
    QList<QGraphicsItem*> items = this->items();
    for (QGraphicsItem* item : items) {
        if (dynamic_cast<Obstaculo*>(item)) {
            removeItem(item);
            delete item;
        }
    }

    // Reiniciar timers
    temporizadorScroll->start();
    temporizadorObstaculos->start(2000);
    temporizadorColisiones->start(16);

    // Asegurar que Goku tiene el foco
    goku->setFocus();
}

void Nivel1::iniciarNivel() {
    iniciarAnimacionFondo();

    // Iniciar generación de obstáculos
    temporizadorObstaculos = new QTimer(this);
    connect(temporizadorObstaculos, &QTimer::timeout, this, &Nivel1::generarObstaculo);
    temporizadorObstaculos->start(2000);

    // Iniciar detección de colisiones
    temporizadorColisiones = new QTimer(this);
    connect(temporizadorColisiones, &QTimer::timeout, this, &Nivel1::verificarColisiones);
    temporizadorColisiones->start(16);

    goku->setFocus();
}

void Nivel1::configurarEscena() {
    // Configurar fondo
    imagenFondo = QPixmap(":/new/prefix1/recursos/fn1.png");
    if (imagenFondo.isNull()) {
        qDebug() << "No se encontró fn1.png, usando fondo por defecto";
        NivelBase::configurarEscena();
    } else {
        QSize sceneSize = sceneRect().size().toSize();
        QPixmap fondoEscalado = imagenFondo.scaled(sceneSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        fondoItem = new QGraphicsPixmapItem(fondoEscalado);
        fondoItem->setPos(0, 0);
        fondoItem->setZValue(-1);
        addItem(fondoItem);

        fondoSecundario = new QGraphicsPixmapItem(fondoEscalado);
        fondoSecundario->setPos(fondoEscalado.width(), 0);
        fondoSecundario->setZValue(-1);
        addItem(fondoSecundario);
    }

    // Configurar Goku
    goku = new Goku(nullptr, this);
    qreal gokuHeight = goku->boundingRect().height();
    goku->setPos(50, sceneRect().height() - gokuHeight - 100);
    addItem(goku);
    goku->setFocus();
    goku->setFlag(QGraphicsItem::ItemIsFocusable);
    goku->agregarVidasAEscena(this);

    // Configurar texto de puntaje
    textoPuntaje = new QGraphicsTextItem();
    textoPuntaje->setPlainText("Puntaje: ");
    textoPuntaje->setDefaultTextColor(Qt::white);
    textoPuntaje->setFont(QFont("Arial", 16));
    textoPuntaje->setPos(sceneRect().width() - 150, 10);
    textoPuntaje->setZValue(10);
    addItem(textoPuntaje);

    // Inicializar timers
    temporizadorScroll = new QTimer(this);
    temporizadorObstaculos = new QTimer(this);
    temporizadorColisiones = new QTimer(this);
}

void Nivel1::iniciarAnimacionFondo() {
    connect(temporizadorScroll, &QTimer::timeout, this, [=]() {
        fondoItem->moveBy(-velocidadScroll, 0);
        fondoSecundario->moveBy(-velocidadScroll, 0);

        int anchoFondo = fondoItem->pixmap().width();
        if (fondoItem->x() <= -anchoFondo) {
            fondoItem->setPos(fondoSecundario->x() + anchoFondo, 0);
        }
        if (fondoSecundario->x() <= -anchoFondo) {
            fondoSecundario->setPos(fondoItem->x() + anchoFondo, 0);
        }
    });
    temporizadorScroll->start(30);
}

void Nivel1::generarObstaculo() {
    Obstaculo::Tipo tipo = (QRandomGenerator::global()->bounded(100) < 60)
    ? Obstaculo::NAVE
    : Obstaculo::PERRO;

    Obstaculo* obstaculo = new Obstaculo(tipo);
    obstaculo->setVelocidad(velocidadObstaculos);

    qreal yPos = (tipo == Obstaculo::NAVE)
                     ? sceneRect().height() * 0.62
                     : sceneRect().height() - 170;

    obstaculo->setPos(sceneRect().width(), yPos);
    addItem(obstaculo);

    connect(obstaculo, &Obstaculo::destroyed, this, [this, tipo]() {
        if (puntaje < 150) { // Solo aumentar puntaje si el juego no ha terminado
            aumentarPuntaje((tipo == Obstaculo::NAVE) ? 2 : 1);
        }
    });
}

void Nivel1::verificarColisiones() {
    if (!goku) return;

    QList<QGraphicsItem*> items = this->items();
    foreach(QGraphicsItem *item, items) {
        if (Obstaculo* obstaculo = dynamic_cast<Obstaculo*>(item)) {
            if (goku->collidesWithItem(obstaculo, Qt::IntersectsItemShape)) {
                disconnect(obstaculo, &Obstaculo::destroyed, this, nullptr);

                int danio = (obstaculo->getTipo() == Obstaculo::NAVE) ? 2 : 1;
                goku->vida -= danio;
                goku->actualizarVidasUI();

                removeItem(obstaculo);
                obstaculo->deleteLater();

                if (goku->vida <= 0) {
                    mostrarMensajeFinal("You Lose!", Qt::red, "Reintentar");
                    emit nivelFallido();
                }
                break;
            }
        }
    }
}

void Nivel1::aumentarPuntaje(int puntos) {
    puntaje += puntos;
    textoPuntaje->setPlainText("Puntaje: " + QString::number(puntaje));

    if (puntaje >= 50) {
        mostrarMensajeFinal("You Win!", Qt::green, "Siguiente Nivel");
        emit nivelCompletado();
        return;
    }

    // Aumentar dificultad cada 10 puntos
    if (puntaje > 0 && puntaje % 10 == 0) {
        velocidadObstaculos += 5;
        int nuevoIntervalo = qMax(800, 2000 - (puntaje/10)*200);
        temporizadorObstaculos->setInterval(nuevoIntervalo);
    }
}

void Nivel1::redimensionarFondo() {
    if (!fondoItem || !fondoSecundario || imagenFondo.isNull()) return;

    QSize sceneSize = sceneRect().size().toSize();
    QPixmap fondoEscalado = imagenFondo.scaled(sceneSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    fondoItem->setPixmap(fondoEscalado);
    fondoSecundario->setPixmap(fondoEscalado);

    if (fondoItem->x() < fondoSecundario->x()) {
        fondoItem->setPos(0, 0);
        fondoSecundario->setPos(fondoEscalado.width(), 0);
    } else {
        fondoSecundario->setPos(0, 0);
        fondoItem->setPos(fondoEscalado.width(), 0);
    }
}
