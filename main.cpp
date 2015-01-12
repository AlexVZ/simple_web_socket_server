
#include "mainwindow.h"
#include <QApplication>

#include "simplewebsocketserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    SimpleWebSocketServer server(w.getTextEditWindow(), w.getConnectionsCombo());
    if(!server.listen(QHostAddress::Any, 1111)) {
        QString err = QString("Unable to start the server: %1.").arg(server.errorString());
        fprintf(stderr, "%s\n", err.toStdString().c_str());
        return -1;
    }
    QObject::connect(&w, SIGNAL(send_to_thread(int, QString)), &server, SLOT(send_to_thread_slot(int, QString)));

    w.show();

    return a.exec();
}
