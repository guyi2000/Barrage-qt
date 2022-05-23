#include <QString>
#include <QRandomGenerator>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "barrage_item.h"
#include "barrage_screen.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Server");

    initServer();

    XYBarrageScreen::getScreen()->show();
    XYBarrageScreen::getScreen()->setMaxBarrageNumber(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

XYBarrageItem *MainWindow::getItem(QString drawText) {
    XYContents *contents = new XYContents(drawText);
    contents->next = new XYContents(XYContents::LF);
    XYBarrageItem *item = new XYBarrageItem(contents);
    QColor c(QRandomGenerator::global()->bounded(256),
             QRandomGenerator::global()->bounded(256),
             QRandomGenerator::global()->bounded(256));
    QFont font("微软雅黑");
    font.setPointSize(30);
    item->setTextFont(font);
    item->setTextColor(c);
    item->setShowTimes(3500);
    item->setStartPos(QPoint(1980, qMin(QRandomGenerator::global()->bounded(54) * 15, 800)));
    item->setAnimation(QEasingCurve::Type(0));

    return item;
}

void MainWindow::initServer() {
    server = new QTcpServer(this);

    connect(ui->startListen, &QPushButton::clicked, [this]{
        if (server -> isListening()) {
            closeServer();
            qDebug() << QString("Stop listening successfully.");
            ui->startListen->setText("启动监听");
        } else {
            const QString address_text = "Any";
            const QHostAddress address = (address_text == "Any") ? QHostAddress::Any : QHostAddress(address_text);
            const unsigned short port = 8897;
            if (server->listen(address, port)) {
                qDebug() << QString("Listening %1:%2 successfully.").arg(address.toString()).arg(port);
                ui->startListen->setText("停止监听");
            }
        }
        updateState();
    });

    connect(server, &QTcpServer::newConnection, this, [this] {
       while(server -> hasPendingConnections()) {
           QTcpSocket *socket = server->nextPendingConnection();
           clientList.append(socket);

           qDebug() << QString("[%1:%2] socket connected!").arg(socket->peerAddress().toString()).arg(socket->peerPort());
           updateState();

           connect(socket, &QTcpSocket::readyRead, [this, socket] {
               if (socket->bytesAvailable() <= 0) return;
               const QString recv_text = QString::fromUtf8(socket->readAll());

               qDebug() << recv_text;
               XYBarrageItem* item = getItem(recv_text);
               XYBarrageScreen::getScreen()->addItem(item);
           });

           connect(socket, &QAbstractSocket::errorOccurred, [socket](QAbstractSocket::SocketError){
              qDebug() << QString("[%1:%2] Soket Error:%3")
                        .arg(socket->peerAddress().toString())
                        .arg(socket->peerPort())
                        .arg(socket->errorString());
           });

           connect(socket, &QTcpSocket::disconnected, [this, socket] {
               socket -> deleteLater();
               clientList.removeOne(socket);
               qDebug() << QString("[%1:%2] socket disonnected")
                           .arg(socket->peerAddress().toString())
                           .arg(socket->peerPort());
               updateState();
           });
       }
    });

    connect(server, &QTcpServer::acceptError, [this](QAbstractSocket::SocketError) {
        qDebug() << "Server Error:" << server->errorString();
    });
}

void MainWindow::updateState() {
    if(server->isListening()){
        setWindowTitle(QString("Server[%1:%2] connections:%3")
                       .arg(server->serverAddress().toString())
                       .arg(server->serverPort())
                       .arg(clientList.count()));
    }else{
        setWindowTitle("Server");
    }
}

void MainWindow::closeServer() {
    server->close();
    for(QTcpSocket * socket:clientList)
    {
        socket->disconnectFromHost();
        if(socket->state()!=QAbstractSocket::UnconnectedState){
            socket->abort();
        }
    }
}
