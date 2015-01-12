
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QComboBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QPlainTextEdit *getTextEditWindow();
    QComboBox *getConnectionsCombo();

protected:
    virtual void keyPressEvent(QKeyEvent *e);

private:
    Ui::MainWindow *ui;

signals:
    void send_to_thread(int, QString);

private slots:
    void on_actionExit_triggered();
    void pushButtonSend_clicked(bool);
};

#endif // MAINWINDOW_H
