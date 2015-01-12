
#ifndef SIMPLEWEBSOCKETSERVER_H
#define SIMPLEWEBSOCKETSERVER_H

#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QPlainTextEdit>
#include <QComboBox>

#include "simplewebsocketthread.h"

class SimpleWebSocketServer : public QWebSocketServer
{
    Q_OBJECT

public:
    SimpleWebSocketServer(QPlainTextEdit *out_window, QComboBox *connections_combo, QObject *parent = 0);
    ~SimpleWebSocketServer();

private:
    QPlainTextEdit *m_out_window;
    QComboBox *m_connections_combo;
    int m_thread_id;
    QList<SimpleWebSocketThread *> m_client_threads;

public slots:
    void deleteLater();
    void send_to_thread_slot(int, QString);

private slots:
    void on_new_connection();
};

#endif // SIMPLEWEBSOCKETSERVER_H
