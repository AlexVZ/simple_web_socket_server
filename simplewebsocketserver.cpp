/****************************************************************************
**
** Copyright (C) 2014 Alex
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

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
