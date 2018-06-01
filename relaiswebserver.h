#pragma once

#include <QObject>

#include "httpserver.h"

class HttpClientConnection;
class HttpRequest;
class RelaisClient;
class RelaisServer;

class RelaisWebserver : public HttpServer
{
    Q_OBJECT

public:
    explicit RelaisWebserver(RelaisServer *relaisServer, QObject *parent = Q_NULLPTR);

protected:
    void handleRequest(HttpClientConnection *connection, const HttpRequest &request) Q_DECL_OVERRIDE;

private:
    void handleRoot(HttpClientConnection *connection, const HttpRequest &request);
    void redirectRoot(HttpClientConnection *connection, const HttpRequest &request);
    void handle404(HttpClientConnection *connection, const HttpRequest &request);
    static QString clientId(const RelaisClient *client, bool forceIp = false);

    RelaisServer *m_relaisServer;
};
