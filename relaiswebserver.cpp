#include "relaiswebserver.h"

#include <QStringBuilder>

#include "httpclientconnection.h"
#include "httpcontainers.h"
#include "relaisserver.h"
#include "relaisclient.h"

RelaisWebserver::RelaisWebserver(RelaisServer *relaisServer, QObject *parent) :
    HttpServer(parent),
    m_relaisServer(relaisServer)
{
}

void RelaisWebserver::handleRequest(HttpClientConnection *connection, const HttpRequest &request)
{
    qDebug() << request.path;

    if(!request.path.startsWith('/'))
    {
        HttpResponse response;
        response.protocol = request.protocol;
        response.statusCode = HttpResponse::StatusCode::BadRequest;
        response.body = "Path does not start with /";
        response.headers.insert(QStringLiteral("Server"), QStringLiteral("Hatschi Server 1.0"));
        response.headers.insert(QStringLiteral("Content-Type"), QStringLiteral("text/html"));

        connection->sendResponse(response);
        return;
    }

    if(request.path == QStringLiteral("/"))
    {
        handleRoot(connection, request);
    }
    else if(request.path.startsWith("/devices/"))
    {
        auto parts = request.path.split('/');
        if(parts.count() != 4)
        {
            handle404(connection, request);
            return;
        }

        RelaisClient *client = Q_NULLPTR;

        for(auto _client : m_relaisServer->clients())
        {
            if(!_client->name().isEmpty() &&
               _client->name() == parts.at(2))
            {
                client = _client;
                break;
            }
        }

        if(!client)
        {
            handle404(connection, request);
            return;
        }

        if(parts.at(3) == "on")
        {
            client->on();
            redirectRoot(connection, request);
            return;
        }
        else if(parts.at(3) == "off")
        {
            client->off();
            redirectRoot(connection, request);
            return;
        }
        else if(parts.at(3) == "toggle")
        {
            client->toggle();
            redirectRoot(connection, request);
            return;
        }
        else
        {
            handle404(connection, request);
            return;
        }
    }
    else
    {
        handle404(connection, request);
    }
}

void RelaisWebserver::handleRoot(HttpClientConnection *connection, const HttpRequest &request)
{
    QString output = "<p>" % tr("%0 clients").arg(m_relaisServer->clients().count()) % "</p>";

    output.append("<table border=\"1\">");
    output.append("<thead>");
    output.append("<tr>");
    output.append("<th>IP-Adress</th>");
    output.append("<th>Name</th>");
    output.append("<th>Status</th>");
    output.append("<th>Actions</th>");
    output.append("</tr>");
    output.append("</thead>");
    output.append("<tbody>");
    for(auto client : m_relaisServer->clients())
    {
        output.append("<tr>");
        output.append("<td>" % client->peerAddress().toString() % ':' % QString::number(client->peerPort()) % "</td>");
        output.append("<td>" % client->name() % "</td>");
        output.append("<td>" % client->status() % "</td>");
        if(!client->name().isEmpty())
        {
            output.append("<td><a href=\"/devices/" % client->name().toHtmlEscaped() % "/toggle\">" % tr("toggle") % "</a> ");
            if(client->status() != QStringLiteral("on"))
                output.append("<a href=\"/devices/" % client->name().toHtmlEscaped() % "/on\">" % tr("on") % "</a> ");
            if(client->status() != QStringLiteral("off"))
                output.append("<a href=\"/devices/" % client->name().toHtmlEscaped() % "/off\">" % tr("off") % "</a> ");
            output.append("</td>");
        }
        output.append("</tr>");
    }
    output.append("</tbody>");
    output.append("</table>");

    HttpResponse response;
    response.protocol = request.protocol;
    response.statusCode = HttpResponse::StatusCode::OK;
    response.body = output.toUtf8();
    response.headers.insert(QStringLiteral("Server"), QStringLiteral("Hatschi Server 1.0"));
    response.headers.insert(QStringLiteral("Content-Type"), QStringLiteral("text/html"));

    connection->sendResponse(response);
}

void RelaisWebserver::redirectRoot(HttpClientConnection *connection, const HttpRequest &request)
{
    HttpResponse response;
    response.protocol = request.protocol;
    response.statusCode = HttpResponse::StatusCode::Found;
    response.body = QByteArrayLiteral("<a href=\"/\">Follow this link</a>");
    response.headers.insert(QStringLiteral("Server"), QStringLiteral("Hatschi Server 1.0"));
    response.headers.insert(QStringLiteral("Content-Type"), QStringLiteral("text/html"));
    response.headers.insert(QStringLiteral("Location"), QStringLiteral("/"));

    connection->sendResponse(response);
}

void RelaisWebserver::handle404(HttpClientConnection *connection, const HttpRequest &request)
{
    HttpResponse response;
    response.protocol = request.protocol;
    response.statusCode = HttpResponse::StatusCode::NotFound;
    response.body = "Not Found";
    response.headers.insert(QStringLiteral("Server"), QStringLiteral("Hatschi Server 1.0"));
    response.headers.insert(QStringLiteral("Content-Type"), QStringLiteral("text/html"));

    connection->sendResponse(response);
}
