#include "nyanspaceapi.h"

NyanSpaceAPI::NyanSpaceAPI(QObject *parent) : QObject(parent)
{
    this->netman = new QNetworkAccessManager(this);
    this->netman->setNetworkAccessible(QNetworkAccessManager::Accessible);
}

void NyanSpaceAPI::getmorepeers()
{
    QNetworkRequest req;
    req.setUrl(NYAN_SPACE_SEEDS);
    reply = this->netman->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(getmorepeers_request_finished()));
}

void NyanSpaceAPI::getmorepeers_request_finished()
{
    auto responseBody = QString(reply->readAll());

    if (responseBody.length() <= 1) {
        QString msg = QString("Unable to fetch peers from %1: %2").arg(NYAN_SPACE_SEEDS).arg(reply->errorString());
        //throw (std::exception(msg.toStdString().c_str()));
        qDebug() << msg;
        throw new std::exception();
    }

    QJsonDocument doc = QJsonDocument::fromJson(responseBody.toUtf8());
    QJsonObject jsonObject = doc.object();
    QJsonArray peers = jsonObject["result"].toArray();

    //Array ret;

    foreach(const QJsonValue &value, peers)
    {
        QJsonObject jsonobj = value.toObject();
        QString addr = jsonobj["addr"].toString();
        AddOneShot(addr.toStdString().c_str());
    }

    qDebug() << QString("%1 peers added from nyan.space.").arg(peers.count());
    emit getmorepeers_finished(peers.count());
}


/*



// vmp32k - experimental seeding method
Value connectnyandotspacepeers(const Array& params, bool fHelp)
{
    if (fHelp) {
        throw runtime_error(
            "connectnyandotspacepeers\n"
            "Fetch and connect to peers via the nyan.space peer list."
        );
    }

    QNetworkAccessManager networkManager;
    networkManager.setNetworkAccessible(QNetworkAccessManager::Accessible);
    QNetworkRequest req;
    req.setUrl(NYAN_SPACE_SEEDS);
    QNetworkReply *reply = networkManager.get(req);

    while(!reply->isFinished()) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    QString responseBody = QString(reply->readAll());
    delete reply;

    if (responseBody.length() <= 1) {
        QString msg = QString("Unable to fetch peers from %1: %2").arg(NYAN_SPACE_SEEDS).arg(reply->errorString());
        throw runtime_error(msg.toStdString().c_str());
    }

    QJsonDocument doc = QJsonDocument::fromJson(responseBody.toUtf8());
    QJsonObject jsonObject = doc.object();
    QJsonArray peers = jsonObject["result"].toArray();

    //Array ret;

    BOOST_FOREACH (const QJsonValue &value, peers)
    {
        QJsonObject jsonobj = value.toObject();
        QString addr = jsonobj["addr"].toString();
        AddOneShot(addr.toStdString().c_str());
    }

    return (QString("%1 peers added from nyan.space.").arg(peers.count())).toStdString().c_str();
}
*/
