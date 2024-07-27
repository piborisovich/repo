#include "portlistener.hpp"

PortListener::PortListener(QObject *parent) :
    QThread(parent)
{
    m_port = new SerialPort();
    sch_frame = 0;
}

void PortListener::run()
{
    while (true){

        int portNum = Find_portXB();
        if (portNum != 0){
            QString portStr;
            /*QString serialNumber = XBConvert::asciiToAddress(Serial_numberH,sizeof(Serial_numberH)) + ":" +
                    XBConvert::asciiToAddress(Serial_numberL,sizeof(Serial_numberL));*/

            portStr = "COM%1";
            portStr = portStr.arg(portNum);
                    //.arg(serialNumber);


            emit(initPort(portStr));
            emit(portConnected(m_port));
            while(true){
                /*if (m_port->Read(buffer, 100, &receivedBytes)){
                    if (receivedBytes != 0){
                        QByteArray message = QByteArray(buffer, receivedBytes);
                        emit(receiveFrame(message));
                    }
                }*/
                receiveData();
                usleep(200000);
            }

        } else {
            QString portStr;
            portStr = "Недоступен";
            emit(initPort(portStr));
        }

        usleep(1000);
    }
}

/*
void PortListener::receiveData()
{
    int cnt;
    if (!m_port->Read((char*)(buffer_rcv), sizeof(buffer_rcv), &cnt)) {
        return;
    }



    if (cnt == -1) {
        cnt = 0;
    }

    if (cnt == 0) {
        return;
    }

    byte data = 0x00;
    int sch_out = 0;

    //QByteArray buff1 = QByteArray::fromRawData((char *)buffer_rcv, cnt); //++++++++++++++
    //qDebug() << XBConvert::addressToString(buff1);
    //emit(receiveFrame(buff));

    while (sch_out != cnt) {
        data = buffer_rcv[sch_out++];
        //qDebug() << "data = " + QString::number(data, 16);
        //qDebug() << "sch_frame = " + QString::number(sch_frame, 16);
        if (sch_frame == 0) {
            if (data == 0x7e) {
                 buf_frame[sch_frame++] = data;
                 buf_frame[sch_frame] = 0xFF;
                 buf_frame[sch_frame + 1] = 0xFF;
             }
        } else {
            buf_frame[sch_frame++] = data;
            if (sch_frame > 3) {
                short int frame_size = (buf_frame[1] << 8) | (buf_frame[2] & 0xFF);
                if (sch_frame == frame_size + 4) {
                    //Фрейм принят полностью
                    //QByteArray buff = QByteArray::fromRawData((char *)buf_frame, sch_frame); //++++++++++++++

                    //qDebug() << XBConvert::addressToString(buff);//buff.resize(sch_frame);
                    //qDebug() << "frame size = " + QString::number(frame_size);
                    //qDebug() << "frame = " + XBConvert::addressToString(buff);
                    char* buff = new char[sch_frame];
                    for (int i = 0; i < sch_frame; i++) {
                        buff[i] = buf_frame[i];
                    }
                    emit(receiveFrame((char*)buff, sch_frame));
                    sch_frame = 0;
                }
            }
        }
    }



 }
*/
 /*

bool PortWriter::sendAnswer(byte frameID, QByteArray outputAddress, int length, QList <byte> params)
{
    byte* outputFrame = new byte[length];
    short int len;
    len = length - 4;
    outputFrame[0] = 0x7E;
    outputFrame[1] = (len >> 8) & 0xFF;
    outputFrame[2] = (byte) len;
    outputFrame[3] = API_TX_Request;
    outputFrame[4] = 0x51;

    for (int i = 0; i < outputAddress.length(); i++){
        outputFrame[i+5] = outputAddress.at(i);
    }
    outputFrame[13] = 0xFF;
    outputFrame[14] = 0xFE;
    outputFrame[15] = 0x00;
    outputFrame[16] = 0x00;
    outputFrame[length - params.count() - 2] = frameID;
    //outputFrame[length - 2] = deviceID;
    for (int i = 0; i < params.count(); i++) {
        outputFrame[length - i - 2] = params.at(i);
    }
    outputFrame[length - 1] = CheckSumm_Calc(outputFrame, length);

    if (!m_port->Write((const char*) outputFrame, length)){
        return false;
    }

    return true;
}

*/
