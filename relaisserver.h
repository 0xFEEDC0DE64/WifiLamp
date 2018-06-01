#pragma once

#include <QObject>
#include <QHostAddress>
#include <QAbstractSocket>
#ifndef QT_NO_NETWORKPROXY
#include <QNetworkProxy>
#endif

class QTcpServer;

class RelaisClient;

class RelaisServer : public QObject
{
    Q_OBJECT

    friend class RelaisClient;

public:
    explicit RelaisServer(QObject *parent = Q_NULLPTR);

    bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);
    void close();

    bool isListening() const;

    void setMaxPendingConnections(int numConnections);
    int maxPendingConnections() const;

    quint16 serverPort() const;
    QHostAddress serverAddress() const;

    qintptr socketDescriptor() const;
    bool setSocketDescriptor(qintptr socketDescriptor);

    bool hasPendingConnections() const;

    QAbstractSocket::SocketError serverError() const;
    QString errorString() const;

    void pauseAccepting();
    void resumeAccepting();

#ifndef QT_NO_NETWORKPROXY
    void setProxy(const QNetworkProxy &networkProxy);
    QNetworkProxy proxy() const;
#endif

    const QSet<RelaisClient*> &clients() const;

Q_SIGNALS:
    void acceptError(QAbstractSocket::SocketError socketError);

protected:
    QSet<RelaisClient*> m_clients;

private Q_SLOTS:
    void newConnection();

private:
    QTcpServer *m_server;
};
