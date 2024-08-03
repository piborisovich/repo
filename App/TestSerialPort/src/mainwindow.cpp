#include "mainwindow.hpp"
#include "ui_mainwindow.h"


const int MainWindow::MAX_PORT_NUMBERS = 20;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    onPortDisconnect();
    delete ui;
}

void MainWindow::init()
{
    QStringList portNames;
    for (int i = 1; i <= MAX_PORT_NUMBERS; ++i) {
        portNames.push_back(QString("COM%1").arg(i));
    }

    for( auto it = getDataTypes().cbegin(); it != getDataTypes().cend(); ++it ) {
        ui->typeRxComboBox->addItem(it.value(), it.key());
        ui->typeTxComboBox->addItem(it.value(), it.key());
    }
    ui->namePortComboBox->addItems(portNames);
    ui->namePortComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->sendButton->setEnabled(false);

    auto result = QObject::connect(ui->connectButton,
                                   &QPushButton::clicked,
                                   this,
                                   &MainWindow::onPortConnect);
    Q_ASSERT(result);

    result = QObject::connect(ui->disconnectButton,
                              &QPushButton::clicked,
                              this,
                              &MainWindow::onPortDisconnect);
    Q_ASSERT(result);

    result = QObject::connect(ui->sendButton,
                              &QPushButton::clicked,
                              this,
                              &MainWindow::onPortSend);
    Q_ASSERT(result);
}

void MainWindow::onPortConnect()
{
    onPortDisconnect();
    m_simplePortListener.reset(new SimplePortListener( {ui->namePortComboBox->currentIndex() + 1, PortConfig::BR_9600, PortConfig::ONE_STOPBIT}));

    auto result = QObject::connect(m_simplePortListener.data(),
                                   &SimplePortListener::portConnected,
                                   this,
                                   &MainWindow::onPortConnected);
    Q_ASSERT(result);

    result = QObject::connect(m_simplePortListener.data(),
                              &SimplePortListener::received,
                              this,
                              &MainWindow::onPortDataReceived);
    Q_ASSERT(result);

    result = QObject::connect(this,
                              &MainWindow::disconnectPort,
                              m_simplePortListener.data(),
                              &SimplePortListener::on_portDisconnect);
    Q_ASSERT(result);

    result = QObject::connect(this,
                              &MainWindow::sendData,
                              m_simplePortListener.data(),
                              &SimplePortListener::onDataSend);
    Q_ASSERT(result);

    m_simplePortListener->start();
}

void MainWindow::onPortDisconnect()
{
    if ( !m_simplePortListener.isNull() ) {
        emit disconnectPort();
        m_simplePortListener->wait();
    }
}

void MainWindow::onPortSend()
{
     int dataType = ui->typeTxComboBox->currentData().toInt();

    QString strData = ui->txTextEdit->toPlainText();

    if ( dataType == HEX_DATA) {
        emit sendData(QByteArray::fromHex(strData.toUtf8()));
    } else {
        emit sendData(strData.toLocal8Bit());
    }
}

void MainWindow::onPortConnected(bool connected)
{
    if ( connected ) {
        ui->connectButton->setEnabled(false);
        ui->disconnectButton->setEnabled(true);
        ui->sendButton->setEnabled(true);
    } else {
        ui->connectButton->setEnabled(true);
        ui->disconnectButton->setEnabled(false);
        ui->sendButton->setEnabled(false);
    }
}

void MainWindow::onPortDataReceived(QByteArray data)
{
    int dataType = ui->typeRxComboBox->currentData().toInt();
    QString strData;
    if ( dataType == HEX_DATA) {
        strData = QString::fromLocal8Bit(data.toHex(':'));
    } else {
        strData = QString::fromLocal8Bit(data);
    }

    ui->rxTextEdit->append(strData);
}
