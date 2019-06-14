#include "networkmanager.h"

NetWorkManager::NetWorkManager(QObject *parent) : QObject(parent)
{
    _flightRouteManager=new QNetworkAccessManager(this);
    Q_CHECK_PTR(_flightRouteManager);
    connect(_flightRouteManager,&QNetworkAccessManager::finished,this,&NetWorkManager::onFinished);
    _flightRouteUrl=QStringLiteral("https://www.utmiss.com/addFlightRoute");
#ifdef HTTPSDEMO
    ///测试http请求
    //一个数组
    //    QJsonArray flightRouteArray;
    //一个对象
    QJsonObject flightRouteJsonData;
    flightRouteJsonData.insert("OrderID", "3HJWJWJ9-20181026-44d655pf");
    flightRouteJsonData.insert("ManufactureID", "1234566789");
    flightRouteJsonData.insert("UASID", "456");
    flightRouteJsonData.insert("TimeStamp", 20180616120530);
    flightRouteJsonData.insert("FlightTime", 320);
    flightRouteJsonData.insert("Coordinate", 1);
    flightRouteJsonData.insert("LongPilot", 121.8563728);
    flightRouteJsonData.insert("Latitude", 34.0448610);
    flightRouteJsonData.insert("Height", 10.00);
    flightRouteJsonData.insert("Altitute", 20.00);
    flightRouteJsonData.insert("GS", 5.0);
    flightRouteJsonData.insert("Course", 68.0);

    // 构建 JSON 文档
    QJsonDocument document;
    document.setObject(flightRouteJsonData);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    //    QString strJson(byteArray);

    //    qDebug() << strJson;
    flightRouteRequest(byteArray);
#else //FTP测试
    flightRouteRequest("");
#endif

}
NetWorkManager::~NetWorkManager()
{
    qDebug("6666");
    if(_flightRouteManager){
        qDebug("0000000");
        _flightRouteManager->disconnect(_flightRouteManager,&QNetworkAccessManager::finished,this,&NetWorkManager::onFinished);
        _flightRouteManager->deleteLater();
    }
}
void NetWorkManager::flightRouteRequest(QByteArray const &requestData)
{
#ifdef HTTPSDEMO
    ///以下是http协议请求
    if(requestData.isEmpty())
        return;
    Q_CHECK_PTR(_flightRouteManager);
    QNetworkRequest request;
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    conf.setProtocol(QSsl::TlsV1SslV3);
    request.setSslConfiguration(conf);
    request.setUrl(QUrl(_flightRouteUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    _flightRouteManager->post(request,requestData);
#else
    ///以下是FTP协议下载
    QUrl m_pUrl;
    m_pUrl.setScheme("ftp");
    m_pUrl.setHost("127.0.0.1");
    m_pUrl.setPort(21);
    m_pUrl.setUserName("zgn");
    m_pUrl.setPassword("123");

    QFileInfo info;
    info.setFile("./1.rar");

    m_file.setFileName("./1.rar");
    m_file.open(QIODevice::WriteOnly | QIODevice::Append);
    m_pUrl.setPath("/Multiple_file_use/20190517.rar");

    _flightRouteManager->get(QNetworkRequest(m_pUrl));
    qDebug()<<"request ok--------"<<m_pUrl;
#endif
}
// 请求完成;
void NetWorkManager::onFinished(QNetworkReply *reply)
{
    QVariant backStauts=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<"back code====== "<<backStauts.toInt();
    QByteArray replyContent = reply->readAll();
#ifdef HTTPSDEMO
    ///以下是http协议请求
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(replyContent, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError) && doucment.isObject()) {
        QJsonObject object = doucment.object();  // 转化为对象
        qDebug()<<"object== "<<object;
        if (object.contains("message")) {  // 包含指定的 key
            QJsonValue value = object.value("message");  // 获取指定 key 对应的 value
            if (value.isString()) {  // 判断 value 是否为字符串
                QString strName = value.toString();  // 将 value 转化为字符串
                qDebug() << "message : " << strName;
            }
        }
    }
#else
    ///以下是FTP协议下载
    Q_CHECK_PTR(reply);
    qDebug()<<replyContent.size();
    m_file.write(replyContent);
    m_file.flush();
    m_file.close();
#endif
    reply->deleteLater();
}
