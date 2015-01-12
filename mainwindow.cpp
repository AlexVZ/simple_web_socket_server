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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mainToolBar->setVisible(false);

    connect(ui->pushButtonSend, SIGNAL(clicked(bool)), this, SLOT(pushButtonSend_clicked(bool)));

    ui->plainTextEdit->setPlainText(tr("waiting connections..."));

    ui->statusBar->showMessage(tr("Ready for connections"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    //ui->statusBar->showMessage(tr("e->key(): %1").arg(e->key())); // 1049 0x419; 1062 0x426
    if( ((e->key() == Qt::Key_X || e->key() == 0x427) && (e->modifiers() == Qt::AltModifier)) || // Alt+X and Alt+Ч
        ((e->key() == Qt::Key_Q || e->key() == 0x419) && (e->modifiers() == Qt::ControlModifier)) || // Ctrl+Q and Ctrl+Й
        ((e->key() == Qt::Key_W || e->key() == 0x426) && (e->modifiers() == Qt::ControlModifier)) ) { // Ctrl+W and Ctrl+Ц
        on_actionExit_triggered();
    }
}

QPlainTextEdit *MainWindow::getTextEditWindow()
{
    return ui->plainTextEdit;
}
QComboBox *MainWindow::getConnectionsCombo()
{
    return ui->comboBox;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::pushButtonSend_clicked(bool)
{
    int thread_id = ui->comboBox->currentText().toInt();
    QString message = ui->lineEdit->text();
    if(thread_id == 0) {
        ui->statusBar->showMessage(tr("Invalid thread id; message NOT sent..."));
        return;
    }
    if(message.size() == 0) {
        ui->statusBar->showMessage(tr("Empty message string; message NOT sent..."));
        return;
    }
    emit send_to_thread(thread_id, message);
}
