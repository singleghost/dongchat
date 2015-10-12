#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork/QUdpSocket>

class Server;

namespace Ui {
class widget;
}

enum MsgType{Msg, UsrEnter, UsrLeft, FileName, Refuse};
class widget : public QWidget
{
    Q_OBJECT

public:
    explicit widget(QWidget *parent = 0, QString usrName = "Nobody");
    ~widget();

private:
    Ui::widget *ui;
    QUdpSocket *udpSocket;
    qint16 port;
    QString uName;
    QString fileName;
    Server *srv;
    QColor color;
protected:
    void usrEnter(QString usrName, QString ipaddr);
    void usrLeft(QString usrName, QString time);
    void sendMsg(MsgType type, QString srvaddr = "");
    void hasPendingFile(QString usrname, QString srvaddr, QString clntaddr, QString fileName);
    QString getIP();
    QString getUsr();
    QString getMsg();
   private slots:
    void processPendingDatagrams();
    void on_sendBtn_clicked();
    void on_exitBtn_clicked();
    void getFileName(QString);
//    void closeEvent(QCloseEvent *);
    void on_sendTBtn_clicked();
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_sizeComboBox_currentIndexChanged(int index);
    void on_boldTBtn_clicked(bool checked);
    void on_italicTBtn_clicked(bool checked);
    void on_underlineTBtn_clicked(bool checked);
    void on_colorTBtn_clicked();

    void closeEvent(QCloseEvent *);
    void on_clearTBtn_clicked();
    void on_saveTBtn_clicked();
};

#endif // WIDGET_H
