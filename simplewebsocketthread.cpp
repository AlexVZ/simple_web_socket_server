
#include "simplewebsocketthread.h"

SimpleWebSocketThread::SimpleWebSocketThread(QPlainTextEdit *out_window, int thread_id, QWebSocket *webSocket, QObject *parent) :
    QThread(parent),
    m_out_window(out_window),
    m_thread_id(thread_id),
    m_webSocket(webSocket)
{
}

SimpleWebSocketThread::~SimpleWebSocketThread()
{
    delete m_webSocket;
}

void SimpleWebSocketThread::run()
{
    while(m_webSocket->isValid()) {
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();
    }
}

int SimpleWebSocketThread::getThreadId()
{
    return m_thread_id;
}

void SimpleWebSocketThread::send_message(QString message)
{
    m_out_window->appendPlainText(tr("thread %1 send message: %2").arg(m_thread_id).arg(message));
    m_webSocket->sendTextMessage(message);
}

void SimpleWebSocketThread::processTextMessage(QString incomming_message)
{
    m_out_window->appendPlainText(tr("thread %1 has incomming message: %2").arg(m_thread_id).arg(incomming_message));
    QStringList args_res_data = incomming_message.split(tr("&"));
    QStringList pair_data;
    QMap<QString, QString> get_args;
    for(int i = 0; i < args_res_data.size(); i++) {
        pair_data = args_res_data[i].split(tr("="));
        if(pair_data.size() == 2) {
            get_args[pair_data[0]] = pair_data[1];
        }
    }

    QString outgoing_message = handle_request(get_args);

    m_out_window->appendPlainText(tr("thread %1 answer: %2").arg(m_thread_id).arg(outgoing_message));
    m_webSocket->sendTextMessage(outgoing_message);
}

QString SimpleWebSocketThread::handle_request(QMap<QString, QString>  &get_args)
{
    if(get_args.size() == 0) {
        return tr("invalid request");
    }
    if(get_args[tr("cmd")] == tr("test")) {
        return tr("result=ok&msg=test ok");
    }
    return tr("result=fail&error=unknown command");
}
