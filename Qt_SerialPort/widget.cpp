#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFixedSize(width(), height());        // Disable maximizing
    QWidget::setWindowTitle("Serial Port"); // Adding Title for widget
    portsComboBoxSetter();
    baudRateComboBoxSetter();
    dataBitsComboBoxSetter();
    stopBitsComboBoxSetter();
    paritiesBitsComboBoxSetter();
    flowControlsComboBoxSetter();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_Disconnect_clicked()
{
    serialPort.close();
}

void Widget::on_pushButton_Clear_clicked()
{
    ui->textBrowser->clear();
}

void Widget::on_pushButton_Connect_clicked()
{
    QString portName = ui->comboBoxPort->currentText();
    serialPort.setPortName(portName);
    serialPort.open(QIODevice::ReadWrite);

    if(!serialPort.isOpen()){
        ui->textBrowser->setTextColor(Qt::red);
        ui->textBrowser->append("!!!! Port is Busy !!!!");
    } else {
        valueInitOnConnectForBaudRate();
        valueInitOnConnectForDataBits();
        valueInitOnConnectForStopBits();
        valueInitOnConnectForParityBits();
        valueInitOnConnectForFlowControl();
    }
}

void Widget::on_pushButton_Refresh_clicked()
{
    ui->comboBoxPort->clear();
    QList<QSerialPortInfo> ports = info.availablePorts();
    QList<QString> stringPorts;
    for(int i = 0 ; i < ports.size() ; i++){
        stringPorts.append(ports.at(i).portName());
    }
    ui->comboBoxPort->addItems(stringPorts);
}

void Widget::on_pushButton_Send_clicked()
{
    QString message = ui->lineEdit_Send->text();
    ui->textBrowser->setTextColor(Qt::darkGreen); // Color of message to send is green.
    ui->textBrowser->append(message);
    serialPort.write(message.toUtf8());
    ui->lineEdit_Send->clear();
}

void Widget::on_lineEdit_Send_returnPressed()
{
    QString message = ui->lineEdit_Send->text();
    ui->textBrowser->setTextColor(Qt::darkMagenta); // Color of message to send is Magenta.
    ui->textBrowser->append(message);
    serialPort.write(message.toUtf8());
    ui->lineEdit_Send->clear();
}

void Widget::paritiesBitsComboBoxSetter()
{
    ui->comboBoxParity->addItem("None");
    ui->comboBoxParity->addItem("Even");
    ui->comboBoxParity->addItem("Odd");
    ui->comboBoxParity->addItem("Mark");
    ui->comboBoxParity->addItem("Space");
}

void Widget::stopBitsComboBoxSetter()
{
    ui->comboBoxStop->addItem("1 Bit");
    ui->comboBoxStop->addItem("1,5 Bits");
    ui->comboBoxStop->addItem("2 Bits");
}

void Widget::dataBitsComboBoxSetter()
{
    ui->comboBoxData->addItem("8");
    ui->comboBoxData->addItem("7");
    ui->comboBoxData->addItem("6");
    ui->comboBoxData->addItem("5");
}

void Widget::flowControlsComboBoxSetter()
{
    ui->comboBoxFlowControl->addItem("None");
    ui->comboBoxFlowControl->addItem("Hardware");
    ui->comboBoxFlowControl->addItem("Software");
}

void Widget::portsComboBoxSetter()
{
    QList<QSerialPortInfo> ports = info.availablePorts();
    QList<QString> stringPorts;
    for(int i = 0 ; i < ports.size() ; i++){
        stringPorts.append(ports.at(i).portName());
    }
    ui->comboBoxPort->addItems(stringPorts);
}

void Widget::baudRateComboBoxSetter()
{
    QList<qint32> baudRates = info.standardBaudRates();
    QList<QString> stringBaudRates;
    for(int i = 0 ; i < baudRates.size() ; i++){
        stringBaudRates.append(QString::number(baudRates.at(i)));
    }
    ui->comboBoxBaudRate->addItems(stringBaudRates);
}

void Widget::valueInitOnConnectForBaudRate()
{
    QString stringbaudRate = ui->comboBoxBaudRate->currentText();
    int intbaudRate = stringbaudRate.toInt();
    serialPort.setBaudRate(intbaudRate);
}

void Widget::valueInitOnConnectForDataBits()
{
    QString dataBits = ui->comboBoxData->currentText();
    if(dataBits == "5 Bits") {
        serialPort.setDataBits(QSerialPort::Data5);
    }
    else if((dataBits == "6 Bits")) {
        serialPort.setDataBits(QSerialPort::Data6);
    }
    else if(dataBits == "7 Bits") {
        serialPort.setDataBits(QSerialPort::Data7);
    }
    else if(dataBits == "8 Bits"){
        serialPort.setDataBits(QSerialPort::Data8);
    }
}

void Widget::valueInitOnConnectForStopBits()
{
    QString stopBits = ui->comboBoxStop->currentText();
    if(stopBits == "1 Bit") {
        serialPort.setStopBits(QSerialPort::OneStop);
    }
    else if(stopBits == "1,5 Bits") {
        serialPort.setStopBits(QSerialPort::OneAndHalfStop);
    }
    else if(stopBits == "2 Bits") {
        serialPort.setStopBits(QSerialPort::TwoStop);
    }
}

void Widget::valueInitOnConnectForParityBits()
{
    QString parity = ui->comboBoxParity->currentText();
    if(parity == "None"){
        serialPort.setParity(QSerialPort::NoParity);
    } else if(parity == "Even"){
        serialPort.setParity(QSerialPort::EvenParity);
    }
    else if(parity == "Odd"){
        serialPort.setParity(QSerialPort::OddParity);
    }
    else if(parity == "Mark"){
        serialPort.setParity(QSerialPort::MarkParity);
    }
    else if(parity == "Space") {
        serialPort.setParity(QSerialPort::SpaceParity);
    }
}

void Widget::valueInitOnConnectForFlowControl()
{
    QString flowControl = ui->comboBoxFlowControl->currentText();
    if(flowControl == "None") {
        serialPort.setFlowControl(QSerialPort::NoFlowControl);
    }
    else if(flowControl == "Hardware") {
        serialPort.setFlowControl(QSerialPort::HardwareControl);
    }
    else if(flowControl == "Software") {
        serialPort.setFlowControl(QSerialPort::SoftwareControl);
    }
    code = ui->lineEditCode->text();
    codeSize = code.size();
    connect(&serialPort,SIGNAL(readyRead()),this,SLOT(receiveMessage()));
}

void Widget::receiveMessage()
{
    QByteArray dataBA = serialPort.readAll();
    QString data(dataBA);
    buffer.append(data);
    int index = buffer.indexOf(code);
    if(index != -1){
        QString message = buffer.mid(0,index);
        ui->textBrowser->setTextColor(Qt::blue); // Receieved message's color is blue.
        ui->textBrowser->append(message);
        buffer.remove(0,index+codeSize);
    }
}

