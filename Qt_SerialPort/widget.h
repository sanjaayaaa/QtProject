#pragma once
#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void receiveMessage();
    void on_pushButton_Connect_clicked();
    void on_pushButton_Disconnect_clicked();
    void on_pushButton_Refresh_clicked();
    void on_pushButton_Clear_clicked();
    void on_pushButton_Send_clicked();
    void on_lineEdit_Send_returnPressed();

private:
    void dataBitsComboBoxSetter();
    void stopBitsComboBoxSetter();
    void paritiesBitsComboBoxSetter();
    void flowControlsComboBoxSetter();
    void portsComboBoxSetter();
    void baudRateComboBoxSetter();
    void valueInitOnConnectForBaudRate();
    void valueInitOnConnectForDataBits();
    void valueInitOnConnectForStopBits();
    void valueInitOnConnectForParityBits();
    void valueInitOnConnectForFlowControl();

private:
    Ui::Widget *ui;
    QSerialPort serialPort;
    QSerialPortInfo info;
    QString buffer;
    QString code;
    int codeSize;

};
