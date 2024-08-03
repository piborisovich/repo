#include "simpleportlistener.hpp"

SimplePortListener::SimplePortListener(const PortConfig &portConfig, QObject *parent) : PortListener(portConfig, parent) {}

void SimplePortListener::onDataSend(QByteArray data)
{
    sendData(data);
}

void SimplePortListener::on_received(char *data, int size)
{
    emit received(QByteArray::fromRawData(data, size));
}
