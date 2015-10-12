#include "server.h"
#include "ui_server.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

Server::Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);

    tPort = 5555;
    tSrv = new QTcpServer(this);
    connect(tSrv, SIGNAL(newConnection()), this, SLOT(sendMsg()) );

    initSrv();
}

Server::~Server()
{
    delete ui;
}

void Server::initSrv()
{
    payloadSize = 64 * 1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesToBeWrite = 0;
    ui -> sStatusLabel -> setText(tr("please choose file to be send"));
    ui -> progressBar -> reset();
    ui -> sOpenBtn -> setEnabled(true);
    ui -> sSendBtn -> setEnabled(false);
    tSrv -> close();
}

void Server::sendMsg()
{
    ui -> sSendBtn -> setEnabled(false);
    clntCoon = tSrv -> nextPendingConnection();
    connect(clntCoon, SIGNAL(bytesWritten(qint64)), this, SLOT(updClntProgress(qint64)) );

    ui -> sStatusLabel -> setText(tr("begin to send file %1").arg(theFileName));

    locFile = new QFile(fileName);
    if(!locFile -> open((QFile::ReadOnly)))
    {
        QMessageBox::information(this, tr("Application"), tr("unable to read the file %1\n%2").arg(fileName).arg(locFile -> errorString()));
        return;
    }

    totalBytes = locFile -> size();
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    time.start();
    QString curFile = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
    sendOut << qint64(0) << qint64(0) << curFile;
    totalBytes += outBlock.size();
    sendOut.device() -> seek(0);
    sendOut << totalBytes << qint64((outBlock.size() - sizeof(qint64) * 2) );
    bytesToBeWrite = totalBytes - clntCoon -> write(outBlock);
    outBlock.resize(0);
}


void Server::updClntProgress(qint64 numBytes)
{
    qApp -> processEvents();
    bytesWritten += (int)numBytes;
    if(bytesToBeWrite > 0)
    {
        outBlock = locFile -> read(qMin(bytesToBeWrite, payloadSize));
        bytesToBeWrite -= (int)clntCoon -> write(outBlock);
        outBlock.resize(0);
    }
    else
        locFile -> close();

    ui -> progressBar -> setMaximum(totalBytes);
    ui -> progressBar -> setValue(bytesWritten);

    float useTime = time.elapsed();
    double speed = bytesWritten / useTime;
    ui -> sStatusLabel -> setText(tr("have been sent: %1MB(%2MB/s)\n total%3MB time used:%4 sec\n approximately remained time:%5 sec").arg(bytesWritten / (1024 * 1024))
                                  .arg(speed*1000 / (1024 * 1024), 0, 'f', 2)
                                  .arg(totalBytes / (1024 * 1024))
                                  .arg(useTime / 1000, 0, 'f', 0)
                                  .arg(totalBytes / speed / 1000 - useTime / 1000, 0, 'f', 0));

    if(bytesWritten == totalBytes)
    {
        locFile -> close();
        tSrv -> close();
        ui -> sStatusLabel -> setText(tr("send file %1 success!").arg(theFileName));
    }
}
void Server::on_sOpenBtn_clicked()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        theFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
        ui -> sStatusLabel -> setText(tr("file to be send is %1").arg(theFileName));
        ui -> sSendBtn -> setEnabled(true);
        ui -> sOpenBtn -> setEnabled(false);
    }
}

void Server::on_sSendBtn_clicked()
{
    if(!tSrv -> listen(QHostAddress::Any, tPort))
    {
        qDebug() << tSrv -> errorString();
        close();
        return;
    }

    ui -> sStatusLabel -> setText(tr("wait for opposite to receive"));
    emit sndFileName(theFileName);

}

void Server::on_sCloseBtn_clicked()
{
    if(tSrv -> isListening())
    {
        tSrv -> close();
        if(locFile -> isOpen())
            locFile -> close();
        clntCoon -> close();
    }
    close();
}

void Server::closeEvent(QCloseEvent *)
{
    on_sCloseBtn_clicked();
}

void Server::refused()
{
    tSrv -> close();
    ui -> sStatusLabel -> setText(tr("opposite refused!"));

}

