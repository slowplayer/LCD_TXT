#include "lcdtxt.h"

LcdTxt::LcdTxt(QWidget *parent)
    : QWidget(parent)
{

    Setui();

}

LcdTxt::~LcdTxt()
{

}
void LcdTxt::Setui()
{
    Ledit=new QLineEdit;
    bt=new QPushButton("浏览");

    Tedit=new QTextEdit;

    serialbox=new QComboBox;
    openbt=new QPushButton("打开串口");
    sendbt=new QPushButton("发送数据");
    quitbt=new QPushButton("退出");

    QHBoxLayout *toplay=new QHBoxLayout;
    toplay->addWidget(Ledit);
    toplay->addWidget(bt);

    QVBoxLayout *btlay=new QVBoxLayout;
    btlay->addWidget(serialbox);
    btlay->addWidget(openbt);
    btlay->addWidget(sendbt);
    btlay->addWidget(quitbt);

    QHBoxLayout *blay=new QHBoxLayout;
    blay->addWidget(Tedit);
    blay->addLayout(btlay);

    QVBoxLayout *mainlay=new QVBoxLayout;
    mainlay->addLayout(toplay);
    mainlay->addLayout(blay);

    setLayout(mainlay);

    Ledit->setReadOnly(true);
    Tedit->setReadOnly(true);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
            QSerialPort serial;
            serial.setPort(info);
            if(serial.open(QIODevice::ReadWrite))
            {
                serialbox->addItem(info.portName());
                serial.close();
            }
    }
    connect(openbt,SIGNAL(clicked()),this,SLOT(slot_open()));
    connect(sendbt,SIGNAL(clicked()),this,SLOT(slot_send()));
    connect(bt,SIGNAL(clicked()),this,SLOT(slot_browse()));
    connect(quitbt,SIGNAL(clicked()),qApp,SLOT(quit()));
}
void LcdTxt::WriteData(int cnt)
{
    myport->write(subcode[cnt]);
}

void LcdTxt::slot_read()
{
    qDebug()<<"Read Data!";
    cnt_num++;
    if(cnt_num<num)
        WriteData(cnt_num);
}

void LcdTxt::slot_open()
{
    myport=new QSerialPort();
    myport->setPortName(serialbox->currentText());
    myport->open(QIODevice::ReadWrite);
    myport->setBaudRate(115200);
    myport->setDataBits(QSerialPort::Data8);
    myport->setParity(QSerialPort::NoParity);
    myport->setStopBits(QSerialPort::OneStop);
    myport->setFlowControl(QSerialPort::NoFlowControl);
    connect(myport,SIGNAL(readyRead()),this,SLOT(slot_read()));
    qDebug()<<"Com open!";
}
void LcdTxt::slot_send()
{
    cnt_num=0;
    WriteData(0);
}

void LcdTxt::slot_browse()
{
    QString path=QFileDialog::getOpenFileName(0,tr("选择TXT文件"),".","文本文件(*.txt);",0,0);
    Ledit->setText(path);
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        txt=stream.readAll();
    }
    file.close();
    int cnt=15-txt.count()%15;
    if(cnt!=15)
    {
        for(int i=0;i<cnt;i++)
            txt+=" ";
    }
    for(int i=0;i<txt.count();i++)
    {
        QString str="";
        str+=txt[i];
        allcode+=MakeFont(str);
    }
    num=0;
    for(int i=0;i<allcode.count();i++)
    {
        if(i&&(i%480==0))num++;
        subcode[num]+=allcode[i];
    }
    if(cnt!=15)num++;
    qDebug()<<txt.count();
    qDebug()<<num;
    qDebug()<<subcode[0].size();
    Tedit->setText(txt);
}
QByteArray LcdTxt::MakeFont(QString str)
{
    QImage image(32,32,QImage::Format_Mono);
    QPainter painter(&image);
    QRgb backcolor=qRgb(255,255,255);
    image.fill(backcolor);
    QFont font;
    font.setFamily("SimSun");
    font.setPixelSize(16);
    font.setWeight(50);
    font.setItalic(false);
    font.setUnderline(false);
    font.setBold(false);
    painter.setFont(font);
    painter.drawText(image.rect(),Qt::AlignCenter |Qt::AlignTop,str);
    QByteArray strdata;
    int tmp=32;
    for(int i=0;i<16;i++)
    {
        for(int j=0;j<2;j++)
        {
            tmp++;
            strdata[i*2+j]=0xff-*(image.bits()+tmp);
        }
        tmp+=2;
    }
    return strdata;
}
