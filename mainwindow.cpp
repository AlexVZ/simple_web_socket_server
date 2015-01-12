
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
