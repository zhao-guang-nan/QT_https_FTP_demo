#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkReply>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfo>
#define HTTPSDEMO//https测试,否则为ftp测试
///https需要把libeay32.dll和ssleay32.dll拷贝到根目录下
class NetWorkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetWorkManager(QObject *parent = 0);
    ~NetWorkManager();

    void flightRouteRequest(QByteArray const &requestData);
signals:

public slots:
    void onFinished(QNetworkReply * reply);


private:
    QNetworkAccessManager* _flightRouteManager=Q_NULLPTR;
    QString     _flightRouteUrl;
    QFile m_file;
};

#endif // NETWORKMANAGER_H
