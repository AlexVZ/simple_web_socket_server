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

#ifndef SIMPLEWEBSOCKETTHREAD_H
#define SIMPLEWEBSOCKETTHREAD_H

#include <QThread>
#include <QtWebSockets/QWebSocket>
#include <QPlainTextEdit>
#include <QTimer>
#include <QEventLoop>

#include "diffiehellmanhelper.h"
#include "aes256helper.h"

class SimpleWebSocketThread : public QThread
{
    Q_OBJECT

public:
    SimpleWebSocketThread(QPlainTextEdit *, int, QWebSocket *, QObject *);
    ~SimpleWebSocketThread();

    void run();
    int getThreadId();
    void send_message(QString);

signals:

private:
    QPlainTextEdit *m_out_window;
    int m_thread_id;
    QWebSocket *m_webSocket;
    DiffieHellmanHelper m_dh_helper;
    AES256Helper m_aes256_helper;
    bool m_dh_completed;

    QString handle_request(QMap<QString, QString> &);
    QString proceed_request(QMap<QString, QString>  &);

public slots:

private slots:
    void is_error(QAbstractSocket::SocketError);
    void state_changed(QAbstractSocket::SocketState);
    void processTextMessage(QString);
};

#endif // SIMPLEWEBSOCKETTHREAD_H
