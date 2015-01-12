
#ifndef SIMPLEWEBSOCKETTHREAD_H
#define SIMPLEWEBSOCKETTHREAD_H

#include <QThread>
#include <QtWebSockets/QWebSocket>
#include <QPlainTextEdit>
#include <QTimer>
#include <QEventLoop>

class SimpleWebSocketThread : public QThread
{
    Q_OBJECT

public:
    SimpleWebSocketThread(QPlainTextEdit *out_window, int thread_id, QWebSocket *webSocket, QObject *parent);
    ~SimpleWebSocketThread();

    void run();
    int getThreadId();
    void send_message(QString message);

signals:

private:
    QPlainTextEdit *m_out_window;
    int m_thread_id;
    QWebSocket *m_webSocket;

    QString handle_request(QMap<QString, QString> &get_args);

public slots:
    void processTextMessage(QString message);
};

#endif // SIMPLEWEBSOCKETTHREAD_H
