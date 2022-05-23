#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTime>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMainWindow>

#include "ui_mainwindow.h"
#include "barrage_item.h"
#include "barrage_screen.h"

#include "barrage_item.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    XYBarrageItem* getItem(QString drawText);

private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QList<QTcpSocket*> clientList;
    void initServer();
    void closeServer();
    void updateState();
};
#endif // MAINWINDOW_H
