#include "relaisserver.h"

#include <QTcpServer>

#include "relaisclient.h"

RelaisServer::RelaisServer(QObject *parent) :
    QObject(parent),
    m_server(new QTcpServer(this))
{
    connect(m_server, &QTcpServer::acceptError, this, &RelaisServer::acceptError);
    connect(m_server, &QTcpServer::newConnection, this, &RelaisServer::newConnection);
}

bool RelaisServer::listen(const QHostAddress &address, quint16 port)
{
    return m_server->listen(address, port);
}

void RelaisServer::close()
{
    m_server->close();
}

bool RelaisServer::isListening() const
{
    return m_server->isListening();
}

void RelaisServer::setMaxPendingConnections(int numConnections)
{
    m_server->setMaxPendingConnections(numConnections);
}

int RelaisServer::maxPendingConnections() const
{
    return m_server->maxPendingConnections();
}

quint16 RelaisServer::serverPort() const
{
    return m_server->serverPort();
}

QHostAddress RelaisServer::serverAddress() const
{
    return m_server->serverAddress();
}

qintptr RelaisServer::socketDescriptor() const
{
    return m_server->socketDescriptor();
}

bool RelaisServer::setSocketDescriptor(qintptr socketDescriptor)
{
    return m_server->setSocketDescriptor(socketDescriptor);
}

bool RelaisServer::hasPendingConnections() const
{
    return m_server->hasPendingConnections();
}

QAbstractSocket::SocketError RelaisServer::serverError() const
{
    return m_server->serverError();
}

QString RelaisServer::errorString() const
{
    return m_server->errorString();
}

void RelaisServer::pauseAccepting()
{
    m_server->pauseAccepting();
}

void RelaisServer::resumeAccepting()
{
    m_server->resumeAccepting();
}

#ifndef QT_NO_NETWORKPROXY
void RelaisServer::setProxy(const QNetworkProxy &networkProxy)
{
    m_server->setProxy(networkProxy);
}

QNetworkProxy RelaisServer::proxy() const
{
    return m_server->proxy();
}
#endif

const QSet<RelaisClient *> &RelaisServer::clients() const
{
    return m_clients;
}

void RelaisServer::newConnection()
{
    auto connection = m_server->nextPendingConnection();
    if(!connection)
        return;

    auto client = new RelaisClient(connection, this);
    m_clients.insert(client);
    connect(client, &QObject::destroyed, this, [=](){
        m_clients.remove(client);
    });
}
