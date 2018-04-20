#pragma once

#include <QObject>
#include <QHostAddress>

class QTcpSocket;

class RelaisServer;

class RelaisClient : public QObject
{
    Q_OBJECT

public:
    explicit RelaisClient(QTcpSocket *socket, RelaisServer *server);

    quint16 localPort() const;
    QHostAddress localAddress() const;
    quint16 peerPort() const;
    QHostAddress peerAddress() const;
    QString peerName() const;

    const QString &name() const;
    const QString &status() const;

public Q_SLOTS:
    void on();
    void off();
    void toggle();
    void requestStatus();

private Q_SLOTS:
    void readyRead();

private:
    QTcpSocket *m_socket;
    RelaisServer *m_server;
    QByteArray m_buffer;
    enum { Name, Status } m_state;

    QString m_name;
    QString m_status;
};
