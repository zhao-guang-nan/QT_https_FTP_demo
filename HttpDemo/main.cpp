#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "networkmanager.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    NetWorkManager *netWork=new NetWorkManager;

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    int ret= app.exec();
    qDebug()<<"--------"<<ret;
    return ret;
}

