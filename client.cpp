#include "client.h"
#include "ui_client.h"
#include <QDebug>
#include <QMessageBox>
#include <QTcpSocket>
Client::Client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Client)
{

    ui->setupUi(this);

    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;

    tClnt = new QTcpSocket(this);
    tport = 5555;
    connect(tClnt, SIGNAL(readyRead()), this, SLOT(readMsg()) );
    connect(tClnt, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayErr(QAbstractSocket::SocketError)) );

}


Client::~Client()
{
    delete ui;
}


void Client::displayErr(QAbstractSocket::SocketError socketErr)
{
    switch(socketErr)
    {
        case QAbstractSocket::RemoteHostClosedError : break;
    default : qDebug() << tClnt -> errorString();
    }
}

void Client::newConn()
{
    blockSize = 0;
    tClnt -> abort();
    tClnt -> connectToHost(hostAddr, tport);
    time.start();
}

void Client::readMsg()
{
    QDataStream in(tClnt);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    float useTime = time.elapsed();

    if(bytesReceived <= sizeof(qint64) * 2)
    {
        if((tClnt -> bytesAvailable() >= sizeof(qint64) * 2) && (fileNameSize == 0))
        {
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
        }
        if((tClnt -> bytesAvailable() >= fileNameSize) && (fileNameSize != 0))
        {
            in >> fileName;
            bytesReceived += fileNameSize;

            if(!locFile -> open(QFile::WriteOnly))
            {
                QMessageBox::information(this, tr("error"), tr("unable to read file%1:\n%2.").arg(fileName).arg(locFile -> errorString()));
                return;
            }
        }
        else
            return;

    }

    if(bytesReceived <= totalBytes)
    {
        bytesReceived += tClnt -> bytesAvailable();
        inBlock = tClnt -> readAll();
        locFile -> write(inBlock);
        inBlock.resize(0);
    }

    ui -> progressBar -> setMaximum(totalBytes);
    ui -> progressBar -> setValue(bytesReceived);

    double speed = bytesReceived / totalBytes;

    ui -> cStatusLabel -> setText(tr("have received: %1MB(%2MB/s)\n total%3MB time used:%4 sec\n approximately remained time:%5 sec").arg(bytesReceived/ (1024 * 1024))
                                  .arg(speed*1000 / (1024 * 1024), 0, 'f', 2)
                                  .arg(totalBytes / (1024 * 1024))
                                  .arg(useTime / 1000, 0, 'f', 0)
                                  .arg(totalBytes / speed / 1000 - useTime / 1000, 0, 'f', 0));

    if(bytesReceived == totalBytes)
    {
        locFile -> close();
        tClnt -> close();
        ui -> cStatusLabel -> setText(tr("receive file %1 completed").arg(fileName));
    }
}

void Client::on_cCancelBtn_clicked()
{
    tClnt -> abort();
    if(locFile -> isOpen())
        locFile -> close();
}

void Client::on_cCloseBtn_clicked()
{
    tClnt -> abort();
    if(locFile -> isOpen())
        locFile -> close();
    close();
}

void Client::closeEvent(QCloseEvent *)
{
    on_cCloseBtn_clicked();
}

void Client::setFileName(QString name)
{
    locFile = new QFile(name);
}

void Client::setHostAddr(QHostAddress addr)
{
    hostAddr = addr;
    newConn();
}
