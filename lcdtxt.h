#ifndef LCDTXT_H
#define LCDTXT_H

#include <QWidget>
#include <QApplication>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QBoxLayout>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QIODevice>
#include <QTextStream>
#include <QByteArray>
#include <QPainter>
#include <QRgb>
#include <QFont>
#include <QImage>
#include <QDebug>
class LcdTxt : public QWidget
{
    Q_OBJECT

public:
    LcdTxt(QWidget *parent = 0);
    ~LcdTxt();
private slots:
    void slot_browse();
    void slot_open();
    void slot_send();
    void slot_read();
private:
    void Setui();
    void WriteData(int cnt);
    QByteArray MakeFont(QString str);
    QPushButton *openbt,*sendbt,*quitbt,*bt;
    QComboBox *serialbox;
    QTextEdit *Tedit;
    QLineEdit *Ledit;
    QSerialPort *myport;
    QString txt;
    QByteArray allcode,subcode[400];
    int num;
    int cnt_num;
    bool waiting;
};

#endif // LCDTXT_H
