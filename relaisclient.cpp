#include "relaisclient.h"

#include <QTcpSocket>

#include "relaisserver.h"

RelaisClient::RelaisClient(QTcpSocket *socket, RelaisServer *server) :
    QObject(server),
    m_socket(socket),
    m_server(server),
    m_state(Name)
{
    m_socket->setParent(this);
    connect(m_socket, &QIODevice::readyRead, this, &RelaisClient::readyRead);
    connect(m_socket, &QAbstractSocket::disconnected, this, &QObject::deleteLater);
}

quint16 RelaisClient::localPort() const
{
    return m_socket->localPort();
}

QHostAddress RelaisClient::localAddress() const
{
    return m_socket->localAddress();
}

quint16 RelaisClient::peerPort() const
{
    return m_socket->peerPort();
}

QHostAddress RelaisClient::peerAddress() const
{
    return m_socket->peerAddress();
}

QString RelaisClient::peerName() const
{
    return m_socket->peerName();
}

const QString &RelaisClient::name() const
{
    return m_name;
}

const QString &RelaisClient::status() const
{
    return m_status;
}

void RelaisClient::on()
{
    m_socket->write(QByteArrayLiteral("1"));
    m_status = QString();
}

void RelaisClient::off()
{
    m_socket->write(QByteArrayLiteral("0"));
    m_status = QString();
}

void RelaisClient::toggle()
{
    m_socket->write(QByteArrayLiteral("t"));
    m_status = QString();
}

void RelaisClient::requestStatus()
{
    m_socket->write(QByteArrayLiteral("s"));
    m_status = QString();
}

void RelaisClient::readyRead()
{
    m_buffer.append(m_socket->readAll());

    int index;
    while((index = m_buffer.indexOf(QByteArrayLiteral("\r\n"))) != -1)
    {
        QString line(m_buffer.left(index));
        m_buffer.remove(0, index + 2);

        switch(m_state)
        {
        case Name: m_name = line; m_state = Status; break;
        case Status: m_status = line; break;
        }
    }
}
