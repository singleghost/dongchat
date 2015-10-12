#include "drawer.h"
#include "ui_drawer.h"

drawer::drawer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::drawer)
{
    ui->setupUi(this);
    connect(ui -> toolButton_1, SIGNAL(clicked(bool)), this, SLOT(showChatWidget1()));
    connect(ui -> toolButton_2, SIGNAL(clicked(bool)), this, SLOT(showChatWidget2()));
    connect(ui -> toolButton_3, SIGNAL(clicked(bool)), this, SLOT(showChatWidget3()));
    connect(ui -> toolButton_4, SIGNAL(clicked(bool)), this, SLOT(showChatWidget4()));
    connect(ui -> toolButton_5, SIGNAL(clicked(bool)), this, SLOT(showChatWidget5()));
    connect(ui -> toolButton_6, SIGNAL(clicked(bool)), this, SLOT(showChatWidget6()));
    connect(ui -> toolButton_7, SIGNAL(clicked(bool)), this, SLOT(showChatWidget7()));
    connect(ui -> toolButton_8, SIGNAL(clicked(bool)), this, SLOT(showChatWidget8()));
    connect(ui -> toolButton_9, SIGNAL(clicked(bool)), this, SLOT(showChatWidget9()));
}

drawer::~drawer()
{
    delete ui;
}

void drawer::showChatWidget1()
{
    chatWidget1 = new widget(0, ui -> toolButton_1 -> text());
    chatWidget1 -> setWindowTitle(ui -> toolButton_1 -> text());
    chatWidget1 -> setWindowIcon(ui -> toolButton_1 -> icon());
    chatWidget1 -> show();
}


void drawer::showChatWidget2()
{
    chatWidget2 = new widget(0, ui -> toolButton_2 -> text());
    chatWidget2 -> setWindowTitle(ui -> toolButton_2 -> text());
    chatWidget2 -> setWindowIcon(ui -> toolButton_2 -> icon());
    chatWidget2 -> show();
}

void drawer::showChatWidget3()
{
    chatWidget3 = new widget(0, ui -> toolButton_3 -> text());
    chatWidget3 -> setWindowTitle(ui -> toolButton_3 -> text());
    chatWidget3 -> setWindowIcon(ui -> toolButton_3 -> icon());
    chatWidget3 -> show();
}

void drawer::showChatWidget4()
{
    chatWidget4 = new widget(0, ui -> toolButton_4 -> text());
    chatWidget4 -> setWindowTitle(ui -> toolButton_4 -> text());
    chatWidget4 -> setWindowIcon(ui -> toolButton_4 -> icon());
    chatWidget4 -> show();
}

void drawer::showChatWidget5()
{
    chatWidget5 = new widget(0, ui -> toolButton_5 -> text());
    chatWidget5 -> setWindowTitle(ui -> toolButton_5 -> text());
    chatWidget5 -> setWindowIcon(ui -> toolButton_5 -> icon());
    chatWidget5 -> show();
}

void drawer::showChatWidget6()
{
    chatWidget6 = new widget(0, ui -> toolButton_6 -> text());
    chatWidget6 -> setWindowTitle(ui -> toolButton_6 -> text());
    chatWidget6 -> setWindowIcon(ui -> toolButton_6 -> icon());
    chatWidget6 -> show();
}

void drawer::showChatWidget7()
{
    chatWidget7 = new widget(0, ui -> toolButton_7 -> text());
    chatWidget7 -> setWindowTitle(ui -> toolButton_7 -> text());
    chatWidget7 -> setWindowIcon(ui -> toolButton_7 -> icon());
    chatWidget7 -> show();
}

void drawer::showChatWidget8()
{
    chatWidget8 = new widget(0, ui -> toolButton_8 -> text());
    chatWidget8 -> setWindowTitle(ui -> toolButton_8 -> text());
    chatWidget8 -> setWindowIcon(ui -> toolButton_8 -> icon());
    chatWidget8 -> show();
}

void drawer::showChatWidget9()
{
    chatWidget9 = new widget(0, ui -> toolButton_9 -> text());
    chatWidget9 -> setWindowTitle(ui -> toolButton_9 -> text());
    chatWidget9 -> setWindowIcon(ui -> toolButton_9 -> icon());
    chatWidget9 -> show();
}
