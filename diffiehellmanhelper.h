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

#ifndef DIFFIEHELLMANHELPER_H
#define DIFFIEHELLMANHELPER_H

#include <QObject>
#include <QtCore>
#include <QPlainTextEdit>

#include <openssl/dh.h>

class DiffieHellmanHelper : public QObject
{
    Q_OBJECT

public:
    DiffieHellmanHelper(QObject *parent = 0);
    ~DiffieHellmanHelper();

    QString start();
    QString startB(QString p, QString g, QString pub_key);
    bool key(QString pub_key_2);
    QString get_key();
    QString get_secret_string();

private:
    QString m_server_secret;

    DH *m_private_key;
    QString m_key;
    BIGNUM *m_pub_key_2;
    bool m_is_bob;

    QString BinaryCharToHexQString(unsigned char* mem, int size);
    int HexQStringToBinaryChar(QString src, unsigned char* mem);
};

#endif // DIFFIEHELLMANHELPER_H
