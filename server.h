#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QTime>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>


namespace Ui {
class Server;
}

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();
    void initSrv();
    void refused();			//close the server

private:
    Ui::Server *ui;
    qint16 tPort;
    QTcpServer *tSrv;
    QString fileName;
    QString theFileName;
    QFile * locFile;

    qint64 totalBytes;
    qint64 bytesWritten;
    qint64 bytesToBeWrite;
    qint64 payloadSize;

    QByteArray outBlock; //buffer the data

    QTcpSocket *clntCoon;	//client connected

    QTime time;
private slots:
    void sendMsg();
    void updClntProgress(qint64 numBytes);	//update the progress bar

    void on_sOpenBtn_clicked();

    void on_sSendBtn_clicked();

    void on_sCloseBtn_clicked();
    void closeEvent(QCloseEvent *);

signals:
    void sndFileName(QString fileName);
};

#endif // SERVER_H
