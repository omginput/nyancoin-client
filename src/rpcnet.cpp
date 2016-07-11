// Copyright (c) 2009-2012 Bitcoin Developers
// Copyright (c) 2011-2012 Litecoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "net.h"
#include "bitcoinrpc.h"

#include <QEventLoop>
#include <QString>
#include <QByteArray>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>


using namespace json_spirit;
using namespace std;

Value getconnectioncount(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getconnectioncount\n"
            "Returns the number of connections to other nodes.");

    LOCK(cs_vNodes);
    return (int)vNodes.size();
}

static void CopyNodeStats(std::vector<CNodeStats>& vstats)
{
    vstats.clear();

    LOCK(cs_vNodes);
    vstats.reserve(vNodes.size());
    BOOST_FOREACH(CNode* pnode, vNodes) {
        CNodeStats stats;
        pnode->copyStats(stats);
        vstats.push_back(stats);
    }
}

Value getpeerinfo(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getpeerinfo\n"
            "Returns data about each connected network node.");

    vector<CNodeStats> vstats;
    CopyNodeStats(vstats);

    Array ret;

    BOOST_FOREACH(const CNodeStats& stats, vstats) {
        Object obj;

        obj.push_back(Pair("addr", stats.addrName));
        obj.push_back(Pair("services", strprintf("%08" PRI64x, stats.nServices)));
        obj.push_back(Pair("lastsend", (boost::int64_t)stats.nLastSend));
        obj.push_back(Pair("lastrecv", (boost::int64_t)stats.nLastRecv));
        obj.push_back(Pair("conntime", (boost::int64_t)stats.nTimeConnected));
        obj.push_back(Pair("version", stats.nVersion));
        obj.push_back(Pair("subver", stats.strSubVer));
        obj.push_back(Pair("inbound", stats.fInbound));
        obj.push_back(Pair("releasetime", (boost::int64_t)stats.nReleaseTime));
        obj.push_back(Pair("startingheight", stats.nStartingHeight));
        obj.push_back(Pair("banscore", stats.nMisbehavior));

        ret.push_back(obj);
    }
    
    return ret;
}

const static QString NYAN_SPACE_SEEDS("http://nyan.space/peers.php?json");
extern void AddOneShot(string strDest);

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
	
	// Need to connect signal somehow and wait for positive response before going on. >.<

    QString responseBody = QString(reply->readAll());
	delete reply;
	
	if (responseBody.length() <= 1) {
		QString msg = QString("Unable to fetch peers from %1: %2").arg(NYAN_SPACE_SEEDS).arg(reply->errorString());
		throw runtime_error(msg.toStdString().c_str());
	}
	
	QJsonDocument doc = QJsonDocument::fromJson(responseBody.toUtf8());
	
	QJsonObject jsonObject = doc.object();
	QJsonArray peers = jsonObject["results"].toArray();
	
	Array ret;
	
	BOOST_FOREACH (const QJsonValue &value, peers)
	{
		QJsonObject jsonobj = value.toObject();
		QString addr = jsonobj["addr"].toString();
		
		Object obj;
        obj.push_back(Pair("addr", addr.toStdString().c_str()));
        ret.push_back(obj);
		
		AddOneShot(addr.toStdString().c_str());
	}

    return ret;
}


