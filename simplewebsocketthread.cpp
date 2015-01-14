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

#include "simplewebsocketthread.h"

SimpleWebSocketThread::SimpleWebSocketThread(QPlainTextEdit *out_window, int thread_id, QWebSocket *webSocket, QObject *parent) :
    QThread(parent),
    m_out_window(out_window),
    m_thread_id(thread_id),
    m_webSocket(webSocket),
    m_dh_completed(false)
{
    connect(m_webSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(processTextMessage(QString)));
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
    if(message.size() == 0) {
        m_out_window->appendPlainText(tr("Thread id: %1. Empty message string; message NOT sent...").arg(m_thread_id));
        return;
    }
    if(m_dh_completed == false) {
        m_out_window->appendPlainText(tr("Thread id: %1. Key exchange not completed; message NOT sent...").arg(m_thread_id));
        return;
    }

    QString encoded_message = tr("cmd=coded&result=ok&msg=%1").arg(m_aes256_helper.encrypt(message, m_dh_helper.get_key()));
    if(encoded_message.size() > 0) {
        m_out_window->appendPlainText(tr("Thread id: %1. Outgoing message: '%2'; encoded: '%3'").arg(m_thread_id).arg(message).arg(encoded_message));
        m_webSocket->sendTextMessage(encoded_message);
    } else {
        m_out_window->appendPlainText(tr("Thread id: %1. Outgoing message ('%2') encode error! Message NOT sent!").arg(m_thread_id).arg(message));
    }
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

    if(outgoing_message.size() > 0) {
        m_out_window->appendPlainText(tr("thread %1 answer: %2").arg(m_thread_id).arg(outgoing_message));
        m_webSocket->sendTextMessage(outgoing_message);
    }
}

QString SimpleWebSocketThread::handle_request(QMap<QString, QString>  &get_args)
{
    if(get_args.size() == 0) {
        return tr("invalid request");
    }
    if(get_args[tr("cmd")] == tr("begin")) {
        QString dh_params = m_dh_helper.start();
        return tr("cmd=begin&result=ok&%1").arg(dh_params);
    } else if(get_args[tr("cmd")] == tr("key")) {
        if(m_dh_helper.key(get_args[tr("pub_key_2")])) {
            QString secret_string = m_dh_helper.get_secret_string();
            if(secret_string.size() == 0) {
                return tr("cmd=key&result=fail&error=unknown error");
            } else {
                return tr("cmd=key&result=ok&check_msg=%1").arg(m_aes256_helper.encrypt(secret_string, m_dh_helper.get_key()));
            }
        } else {
            return tr("cmd=key&result=fail&error=unknown error");
        }
    } else if(get_args[tr("cmd")] == tr("coded")) {
        if(m_dh_completed == false) {
            QString msg_decrypted;
            if(m_aes256_helper.decrypt(get_args[tr("msg")], msg_decrypted, m_dh_helper.get_key())) {
                if(msg_decrypted == m_dh_helper.get_secret_string()) {
                    m_dh_completed = true;
                    return tr("");
                } else {
                    return tr("cmd=coded&result=fail&error=decode error");
                }
            } else {
                return tr("cmd=coded&result=fail&error=decode error");
            }
        } else {
            QString get_args_decrypted_str;
            if(m_aes256_helper.decrypt(get_args[tr("msg")], get_args_decrypted_str, m_dh_helper.get_key())) {
                m_out_window->appendPlainText(tr(" thread %1 decoded message: %2").arg(m_thread_id).arg(get_args_decrypted_str));
                QMap<QString, QString> get_args_decrypted;
                QStringList args_res_data = get_args_decrypted_str.split(tr("&"));
                QStringList pair_data;
                for(int i = 0; i < args_res_data.size(); i++) {
                    pair_data = args_res_data[i].split(tr("="));
                    if(pair_data.size() == 2) {
                        get_args_decrypted[pair_data[0]] = pair_data[1];
                    }
                }
                return tr("cmd=coded&result=ok&msg=%1").arg(m_aes256_helper.encrypt(proceed_request(get_args_decrypted), m_dh_helper.get_key()));
            } else {
                return tr("cmd=coded&result=fail&error=decode error");
            }
        }
    }
    return tr("cmd=%1&result=fail&error=unknown command").arg(get_args[tr("cmd")]);
}

QString SimpleWebSocketThread::proceed_request(QMap<QString, QString>  &get_args)
{
    if(get_args.size() == 0) {
        return tr("invalid request");
    }
    if(get_args[tr("cmd")] == tr("test")) {
        return tr("result=ok&msg=test ok");
    }
    return tr("result=fail&error=unknown command");
}
