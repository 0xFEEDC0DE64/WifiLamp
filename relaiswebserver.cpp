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
        response.headers.insert(QStringLiteral("Server"), QStringLiteral("Hatschi Server 1.0"));
        response.headers.insert(QStringLiteral("Content-Type"), QStringLiteral("text/html"));

        connection->sendResponse(response, tr("Path does not start with /"));
        return;
    }

    if(request.path == QStringLiteral("/"))
    {
        handleRoot(connection, request);
    }
    else if(request.path == QStringLiteral("/refresh"))
    {
        for(auto client : m_relaisServer->clients())
            client->requestStatus();
        redirectRoot(connection, request);
    }
    else if(request.path.startsWith("/devices/"))
    {
        auto parts = request.path.split('/');
        if(parts.count() != 4)
        {
            handle404(connection, request);
            return;
        }

        RelaisClient *client;

        {
            auto iter = std::find_if(m_relaisServer->clients().constBegin(), m_relaisServer->clients().constEnd(),
                                     [&parts](const RelaisClient *client){
                                         return clientId(client) == parts.at(2);
                                     });

            if(iter == m_relaisServer->clients().constEnd())
            {
                handle404(connection, request);
                return;
            }

            client = *iter;
        }

        static const QHash<QString, std::function<void(RelaisClient*)> > actions {
            { QStringLiteral("toggle"),  [](RelaisClient *client){ client->toggle();        } },
            { QStringLiteral("on"),      [](RelaisClient *client){ client->on();            } },
            { QStringLiteral("off"),     [](RelaisClient *client){ client->off();           } },
            { QStringLiteral("refresh"), [](RelaisClient *client){ client->requestStatus(); } },
            { QStringLiteral("reboot"),  [](RelaisClient *client){ client->reboot();        } },
            { QStringLiteral("delete"),  [](RelaisClient *client){ client->deleteLater();   } }
        };

        {
            auto iter = actions.find(parts.at(3));
            if(iter == actions.constEnd())
                handle404(connection, request);
            else
            {
                (*iter)(client);
                redirectRoot(connection, request);
            }
        }
    }
    else
    {
        handle404(connection, request);
    }
}

void RelaisWebserver::handleRoot(HttpClientConnection *connection, const HttpRequest &request)
{
    QString output = "<h1>Lampen-Steuerung</h1>";

    output.append("<a href=\"/refresh\">Alle aktualisieren</a>");

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
        output.append("<td>" % clientId(client, true).toHtmlEscaped() % "</td>");
        output.append("<td>" % client->name().toHtmlEscaped() % "</td>");
        output.append("<td>" % client->status().toHtmlEscaped() % "</td>");
        output.append("<td>");
        output.append("<a href=\"/devices/" % clientId(client).toHtmlEscaped() % "/toggle\">" % tr("Toggle") % "</a> ");
        output.append("<a href=\"/devices/" % clientId(client).toHtmlEscaped() % "/on\">" % tr("On") % "</a> ");
        output.append("<a href=\"/devices/" % clientId(client).toHtmlEscaped() % "/off\">" % tr("Off") % "</a> ");
        output.append("<a href=\"/devices/" % clientId(client).toHtmlEscaped() % "/refresh\">" % tr("Refresh") % "</a> ");
        output.append("<a href=\"/devices/" % clientId(client).toHtmlEscaped() % "/reboot\">" % tr("Reboot") % "</a> ");
        output.append("<a href=\"/devices/" % clientId(client).toHtmlEscaped() % "/delete\">" % tr("Delete") % "</a> ");
        output.append("</td>");
        output.append("</tr>");
    }

    output.append("</tbody>");
    output.append("</table>");

    HttpResponse response;
    response.protocol = request.protocol;
    response.statusCode = HttpResponse::StatusCode::OK;
    response.headers.insert(QStringLiteral("Server"), QStringLiteral("Hatschi Server 1.0"));
    response.headers.insert(QStringLiteral("Content-Type"), QStringLiteral("text/html"));

    connection->sendResponse(response, output);
}

void RelaisWebserver::redirectRoot(HttpClientConnection *connection, const HttpRequest &request)
{
    HttpResponse response;
    response.protocol = request.protocol;
    response.statusCode = HttpResponse::StatusCode::Found;

    response.headers.insert(QStringLiteral("Server"), QStringLiteral("Hatschi Server 1.0"));
    response.headers.insert(QStringLiteral("Content-Type"), QStringLiteral("text/html"));
    response.headers.insert(QStringLiteral("Location"), QStringLiteral("/"));

    connection->sendResponse(response, "<a href=\"/\">" % tr("Follow this link") % "</a>");
}

void RelaisWebserver::handle404(HttpClientConnection *connection, const HttpRequest &request)
{
    HttpResponse response;
    response.protocol = request.protocol;
    response.statusCode = HttpResponse::StatusCode::NotFound;
    response.headers.insert(QStringLiteral("Server"), QStringLiteral("Hatschi Server 1.0"));
    response.headers.insert(QStringLiteral("Content-Type"), QStringLiteral("text/html"));

    connection->sendResponse(response, tr("Not Found"));
}

QString RelaisWebserver::clientId(const RelaisClient *client, bool forceIp)
{
    if(!client->name().isEmpty() && !forceIp)
        return client->name();
    return client->peerAddress().toString() % ':' % QString::number(client->peerPort());
}
