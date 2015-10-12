#ifndef DRAWER_H
#define DRAWER_H

#include <QWidget>
#include <widget.h>

namespace Ui {
class drawer;
}

class drawer : public QWidget
{
    Q_OBJECT

public:
    explicit drawer(QWidget *parent = 0);
    ~drawer();

private:
    Ui::drawer *ui;
private slots:
    void showChatWidget1();
    void showChatWidget2();
    void showChatWidget3();
    void showChatWidget4();
    void showChatWidget5();
    void showChatWidget6();
    void showChatWidget7();
    void showChatWidget8();
    void showChatWidget9();
    
private:
    widget* chatWidget1;
    widget* chatWidget2;
    widget* chatWidget3;
    widget* chatWidget4;
    widget* chatWidget5;
    widget* chatWidget6;
    widget* chatWidget7;
    widget* chatWidget8;
    widget* chatWidget9;
};

#endif // DRAWER_H
