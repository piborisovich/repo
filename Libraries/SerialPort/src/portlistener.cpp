#include "portlistener.h"

PortListener::PortListener(QObject *parent) :
    QThread(parent)
{
    m_port = new QSerialPort();
    sch_frame = 0;
    APIresponse = 0;
    error_XB = 0;
    sch_inf = 0;
    Source_addressL = 0;
    Source_addressH = 0;
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

//Поиск номера порта XBee
 int PortListener::Find_portXB()
 {
     char buf[64];
     int cnt;
     QString s;
     for (int p = 1; p < MAX_SCAN_DEVICES; p++){
         if(!m_port->Openu(p, 9600)){
             continue;
         }

         //Вход в командный режим
        msleep(1000);

         byte s_en[] = "+++";
         if ( !m_port->Write((const char*)s_en,3)) { m_port->Close(); continue; }

         msleep(1000);

         m_port->Read(buf, sizeof(buf), &cnt);
         if (cnt == 3){
             s = QString::fromAscii(buf, cnt);

             if (s != "OK\r"){
                 m_port->Close();
                 s.clear();
                 continue;
             }

             s.clear();

             if (Init_XB()){
                 return p;
             }
         }
         m_port->Close();
        //return p;
     }
     return 0;
 }

 bool PortListener::Init_XB ()
 {
     byte res;

     //Запрос серийного номера (младшая часть)
     com_AT[0]='S';
     com_AT[1]='L';
     if (!(res = Send_ATcommand(_read, 8, Receive_frame))){
         return false;
     }

     for (byte i = 0; i < res - 1; i++){
         Serial_numberL[i] = Receive_frame[i];
     }

     //Запрос серийного номера (старшая часть)
     com_AT[0]='S';
     com_AT[1]='H';
     if (!(res = Send_ATcommand (_read, 8, Receive_frame))){
         return false;
     }

     byte n = 8 - (res - 1);//i;
     for (byte i = 0; i < res-1; i++){
         Serial_numberH[n + i] = Receive_frame[i];
     }

     for (byte i = 0; i < n; i++){
         Serial_numberH[i] = '0';
     }

     //PAN ID
     com_AT[0]='I';
     com_AT[1]='D';//0x0457 = 1111 dec
     if (!(res = Send_ATcommand(_read, 16, Receive_frame))){
         return false;
     }

     if (memcmp(Receive_frame, ID, res - 1) != 0) {

         //Start Initialization XB

         //Write PAN ID
         memcpy(Transmit_frame, ID, 16);
         if (!Send_ATcommand(_write_param, 16, Transmit_frame)){
            return false;
         }

         //Node Identifier max=20
         com_AT[0]='N';
         com_AT[1]='I';
         memcpy(Transmit_frame, NI, sizeof(NI));

         if (!Send_ATcommand(_write_param, sizeof(NI), Transmit_frame)){
             return false;
         }

         //Coordinator Enable
         com_AT[0]='C';
         com_AT[1]='E';
         Transmit_frame[0] = '1';
         if (!Send_ATcommand (_write_param, 1, Transmit_frame)){
             return false;
         }

         //API Enable
         com_AT[0]='A';
         com_AT[1]='P';
         Transmit_frame[0] = '1';
         if (!Send_ATcommand (_write_param, 1, Transmit_frame)){
             return false;
         }

         com_AT[0]='W';
         com_AT[1]='R';
         if (!Send_ATcommand(_write, 0, Transmit_frame)){
             return false;
         }
     }

      /*  com_AT[0]='A';
         com_AT[1]='C';
         if (!Send_ATcommand(_write, 0, Transmit_frame)){
             return false;
         }
     }*/

     //Exit_ATcomm_mode();
     com_AT[0] = 'C';
     com_AT[1] = 'N';
     if (!Send_ATcommand(_write, 0, Transmit_frame)){
         return false;
     }

     return true;
 }

 byte PortListener::Send_ATcommand(byte param, byte length, byte *adr)
 {
     byte st1[10];
     st1[0] = 'A';
     st1[1] = 'T';
     st1[2] = com_AT[0];
     st1[3] = com_AT[1];
     st1[4]= ' ';

     st1[5]=0x0d;

     if (!m_port->Write((const char*)st1, 5)){
         return 0;//false;
     }

     if (param == _write_param){
         // with param
         if (!m_port->Write((const char*)adr,length)){
             //	ERR_TYPE=7;
             return 0;//false;
         }
     }

     if (!m_port->Write((const char*)&st1[5],1)){
         //	ERR_TYPE=7;//
         return 0;//false;
     }

     msleep(200);

     int cnt = 0;

     if (!m_port->Read((char*)adr,128,&cnt)){
         return 0;//false;
     }

     if (cnt == 0 || cnt == -1){
         //	ERR_TYPE=7;//
         return 0;//false;
     }

     if(*adr == 'E' && *(adr+1) == 'R'){
         return 0;//false;
     }

     if(param == _read){
         return cnt;
     }

     return 1;//true;
 }

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

 byte* PortListener::Receive_APIframe (byte *address, byte cnt)
 {
     //flags.yes_answer =0;
     byte *ptr_in = address;
     APIresponse = *ptr_in++;
     byte flag_err =0;

     switch (APIresponse)
     {
     case API_Com_Response:
         if (*ptr_in++ != 'C') {flag_err =1; break;}
         if (*ptr_in++ != com_AT[0])  {flag_err =1; break;}
         if (*ptr_in++ != com_AT[1])  {flag_err =1; break;}
         if (*ptr_in++ != 0)  {flag_err =1; break;}	// != OK
         if (cnt > 5)
         {
             error_XB =0;
             sch_inf = cnt -5;
             return ptr_in;	//is command data
         }
         break;
     case API_Modem_Status:
         break;
     case API_TX_Status:
         if (*ptr_in++ != 'T')  {flag_err =1; break;}
         ptr_in +=3;
         if (*ptr_in++ != 0)  {flag_err =1; break;}	// != OK
         break;
     case API_RX_Packet:
         sch_inf = cnt - 12;	//-
         Source_addressH = *(word32*)ptr_in;
         ptr_in +=4;
         Source_addressL = *(word32*)ptr_in;
         ptr_in += 4+3;
         break;

     default: return 0;
     }
     if (flag_err)
     {
         error_XB = err_transaction;
         return (byte*)-1;
     }
     error_XB =0;
     if (APIresponse == API_RX_Packet)
         return ptr_in;
     return 0;
 }
