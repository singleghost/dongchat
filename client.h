#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QHostAddress>
#include <QFile>
#include <QTime>
#include <QtNetwork/QTcpSocket>

class QTcpSocket;

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

    void setHostAddr(QHostAddress addr);
    void setFileName(QString name);
protected:
    void closeEvent(QCloseEvent *);
private:
    Ui::Client *ui;
    QTcpSocket *tClnt;
    quint16 blockSize;
    QHostAddress hostAddr;
    qint16 tport;

    qint64 totalBytes;
    qint64 bytesReceived;
    qint64 fileNameSize;
    QString fileName;
    QFile *locFile;
    QByteArray inBlock;

    QTime time;
private slots:
    void newConn();
    void readMsg();
    void displayErr(QAbstractSocket::SocketError socketErr);

    void on_cCancelBtn_clicked();
    void on_cCloseBtn_clicked();
};

#endif // CLIENT_H
