#include "widget.h"
#include "server.h"
#include <client.h>
#include <QFileDialog>
#include "ui_widget.h"
#include <QtNetwork/QHostInfo>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>
#include <QtNetwork/QNetworkInterface>
#include <QProcess>
#include <QtNetwork/QUdpSocket>
#include <QCloseEvent>
#include <QColorDialog>
#include <QDialog>
widget::widget(QWidget *parent, QString usrName) :
    QWidget(parent),
    ui(new Ui::widget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    uName = usrName;
    udpSocket = new QUdpSocket(this);
    port = 23232;
    udpSocket -> bind(port, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    srv = new Server(this);
    connect(srv, SIGNAL(sndFileName(QString)), this, SLOT(getFileName(QString)) );
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()) );
    sendMsg(UsrEnter);
}


widget::~widget()
{
    delete ui;
}

void widget::sendMsg(MsgType type, QString srvaddr)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    QString address = getIP();
    out << type << getUsr();

    switch(type)
    {
        case Msg:
            if(ui -> msgTextEdit -> toPlainText() == ""){
                QMessageBox::information(this, tr("error!"), tr("messages can't be null!"), QMessageBox::Ok);
                return;
            }
            out << address << getMsg();
            ui -> msgBrowser -> verticalScrollBar() -> setValue(ui -> msgBrowser ->
                                                                verticalScrollBar() -> maximum());
        break;

    case UsrEnter:
        out << address;
        break;
    case UsrLeft:
        break;
    case FileName: {
        int row = ui -> usrTableWidget -> currentRow();
        QString clntaddr = ui -> usrTableWidget -> item(row, 1) -> text();
        out << address << clntaddr << fileName;
        break;
    }
    case Refuse:
        out << srvaddr;
        break;
    }//switch end
          udpSocket -> writeDatagram(data, data.length(), QHostAddress::Broadcast, port) ;
}


void widget::processPendingDatagrams()
{
    while(udpSocket -> hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket -> pendingDatagramSize());
        udpSocket -> readDatagram(datagram.data(), datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);
        int msgType;
        in >> msgType;
        QString usrName, ipAddr, msg;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        switch(msgType)
        {
            case Msg:
                in >> usrName >> ipAddr >> msg;
                ui -> msgBrowser -> setTextColor(Qt::blue);
                ui -> msgBrowser -> setCurrentFont(QFont("Times New Roman", 12));
                ui -> msgBrowser -> append("[ " + usrName + " ] " + time );
                ui -> msgBrowser -> append(msg);
            break;
        case UsrEnter:
                in >> usrName >> ipAddr;
                usrEnter(usrName, ipAddr);
            break;
        case UsrLeft:
            in >> usrName;
            usrLeft(usrName,time);
            break;
        case FileName: {
            in >> usrName >> ipAddr;
            QString clntAddr , fileName;
            in >> clntAddr >> fileName;
            hasPendingFile(usrName, ipAddr, clntAddr, fileName);
            break;
        }
        case Refuse: {
            in >> usrName;
            QString srvAddr;
            in >> srvAddr;
            QString ipAddr = getIP();

            if(ipAddr == srvAddr)
                srv -> refused();
            break;

        }// case refuse end
        }
    }
}

void widget::usrEnter(QString usrName, QString ipaddr)
{
    bool isEmpty = ui -> usrTableWidget -> findItems(usrName, Qt::MatchExactly).isEmpty();
    if(isEmpty)
    {
        QTableWidgetItem *usr = new QTableWidgetItem(usrName);
        QTableWidgetItem *ip= new QTableWidgetItem(ipaddr);

        ui -> usrTableWidget -> insertRow(0);
        ui -> usrTableWidget -> setItem(0,0,usr);
        ui -> usrTableWidget -> setItem(0,1,ip);
        ui -> msgBrowser -> setTextColor(Qt::gray);
        ui -> msgBrowser -> setCurrentFont(QFont("Times New Roman", 10));
        ui -> usrNumLabel -> setText(tr("onlineNo: %1").arg(ui -> usrTableWidget -> rowCount()));

        sendMsg(UsrEnter);
    }

}

void widget::usrLeft(QString usrName, QString time)
{
    int rowNum = ui -> usrTableWidget -> findItems(usrName, Qt::MatchExactly).first() -> row();
    ui -> usrTableWidget -> removeRow(rowNum);
    ui -> msgBrowser -> setTextColor(Qt::gray);
    ui -> msgBrowser -> setCurrentFont(QFont("Times New Roman", 10));
    ui -> msgBrowser -> append(tr("%1 leaves at %2").arg(usrName).arg(time));
    ui -> usrNumLabel -> setText(tr("onlineNo: %1").arg(ui -> usrTableWidget -> rowCount()));
}

QString widget::getIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, list)
    {
       if(addr.protocol() == QAbstractSocket::IPv4Protocol)
           return addr.toString();
    }
        return 0;
}

QString widget::getUsr()
{
    return uName;

}

QString widget::getMsg()
{
    QString msg = ui -> msgTextEdit -> toHtml();
    ui -> msgTextEdit -> clear();
    ui -> msgTextEdit -> setFocus();
    return msg;
}
void widget::on_sendBtn_clicked()
{
    sendMsg(Msg);
}

void widget::on_exitBtn_clicked()
{
    sendMsg(UsrLeft);
    close();

}

void widget::getFileName(QString name)
{
   fileName = name;
   sendMsg(FileName);
}
void widget::on_sendTBtn_clicked()
{
    if(ui -> usrTableWidget -> selectedItems().isEmpty())
    {
        QMessageBox::warning(0,tr("choose user"), tr("please choose a user"), QMessageBox::Ok);
        return;
    }
    srv -> show();
    srv -> initSrv();
}

void widget::hasPendingFile(QString usrname, QString srvaddr, QString clntaddr, QString fileName)
{
    QString ipAddr = getIP();
    if(ipAddr == clntaddr)
    {
        int btn = QMessageBox::information(this, tr("receive file"), tr("coming from %1(%2) file:%3, receive it or not?").arg(usrname).arg(srvaddr).arg(fileName), QMessageBox::Yes, QMessageBox::No);
        if(btn == QMessageBox::Yes)
        {
            QString name = QFileDialog::getSaveFileName(0, tr("save file"), fileName);
            if(!name.isEmpty())
            {
                Client *clnt = new Client(this);
                clnt -> setFileName(name);
                clnt -> setHostAddr(QHostAddress(srvaddr));
                clnt -> show();
            }
        }
        else
            sendMsg(Refuse, srvaddr);
    }
}

void widget::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui -> msgTextEdit -> setFont(f);
    ui -> msgTextEdit -> setFocus();
}

void widget::on_sizeComboBox_currentIndexChanged(int index)
{
    ui -> msgTextEdit -> setFontPointSize(double(index));
    ui -> msgTextEdit -> setFocus();
}

void widget::on_boldTBtn_clicked(bool checked)
{
    if(checked)
        ui -> msgTextEdit -> setFontWeight(QFont::Bold);
    else
        ui -> msgTextEdit -> setFontWeight(QFont::Normal);
    ui -> msgTextEdit -> setFocus();
}


void widget::on_italicTBtn_clicked(bool checked)
{
        ui -> msgTextEdit -> setFontItalic(checked);
        ui -> msgTextEdit -> setFocus();
}

void widget::on_underlineTBtn_clicked(bool checked)
{
    ui -> msgTextEdit -> setFontUnderline(checked);
    ui -> msgTextEdit -> setFocus();
}

void widget::on_colorTBtn_clicked()
{
    color = QColorDialog::getColor(color, this);
    if(color.isValid())
    {
        ui -> msgTextEdit -> setTextColor(color);
        ui -> msgTextEdit -> setFocus();
    }
}

void widget::closeEvent(QCloseEvent * event)
{
    sendMsg(UsrLeft);

}

void widget::on_clearTBtn_clicked()
{
   ui -> msgBrowser -> clear();
}

void widget::on_saveTBtn_clicked()
{
    QString saveFileName;
    saveFileName =QFileDialog::getSaveFileName(this);
    QFile sFile(saveFileName);
    if(!sFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&sFile);
    QTextDocument *doc = ui -> msgBrowser -> document();
    out << doc->toHtml();

    sFile.close();
}
