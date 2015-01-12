
#include "simplewebsocketserver.h"
#include "simplewebsocketthread.h"

SimpleWebSocketServer::SimpleWebSocketServer(QPlainTextEdit *out_window, QComboBox *connections_combo, QObject *parent) :
    QWebSocketServer(tr("SimpleWebSocketServer"), QWebSocketServer::NonSecureMode, parent),
    m_out_window(out_window),
    m_connections_combo(connections_combo),
    m_thread_id(0)
{
    connect(this, SIGNAL(newConnection()), this, SLOT(on_new_connection()));
}

SimpleWebSocketServer::~SimpleWebSocketServer()
{
    for(int i = 0; i < m_client_threads.size(); i++) {
        delete m_client_threads[i];
    }
}

void SimpleWebSocketServer::on_new_connection()
{
    m_thread_id++;
    QWebSocket *web_socket = nextPendingConnection();

    SimpleWebSocketThread *thread = new SimpleWebSocketThread(m_out_window, m_thread_id, web_socket, this);

    m_client_threads.push_back(thread);
    m_connections_combo->addItem(tr("%1").arg(m_thread_id));
    m_out_window->appendPlainText(tr("New client connected; Thread id: %1.").arg(m_thread_id));

    connect(web_socket, SIGNAL(textMessageReceived(QString)), thread, SLOT(processTextMessage(QString)));
    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));

    thread->start();
}

void SimpleWebSocketServer::deleteLater()
{
    SimpleWebSocketThread *thread = (SimpleWebSocketThread *)(QObject::sender());
    int thread_id = thread->getThreadId();
    for(int i = 0; i < m_connections_combo->count(); i++) {
        if(m_connections_combo->itemText(i).toInt() == thread_id) {
            m_connections_combo->removeItem(i);
            break;
        }
    }
    for(int i = 0; i < m_client_threads.size(); i++) {
        if(m_client_threads[i] == thread) {
            m_client_threads.removeAt(i);
            break;
        }
    }
    disconnect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
    delete thread;
    m_out_window->appendPlainText(tr("Client disconnected; Thread id: %1.").arg(thread_id));
}

void SimpleWebSocketServer::send_to_thread_slot(int thread_id, QString message)
{
    for(int i = 0; i < m_client_threads.size(); i++) {
        if(m_client_threads[i]->getThreadId() == thread_id) {
            m_client_threads[i]->send_message(message);
            break;
        }
    }
}
