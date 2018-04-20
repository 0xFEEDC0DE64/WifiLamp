#include <QCoreApplication>
#include <QTcpServer>

#include "relaisserver.h"
#include "relaiswebserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    RelaisServer server(&app);
    if(!server.listen(QHostAddress::Any, 1234))
    {
        qCritical() << "server could not start listening:" << server.errorString();
        return -1;
    }

    RelaisWebserver webServer(&server, &app);
    if(!webServer.listen(QHostAddress::Any, 8080))
    {
        qCritical() << "webserver could not start listening:" << webServer.errorString();
        return -2;
    }

    return app.exec();
}
