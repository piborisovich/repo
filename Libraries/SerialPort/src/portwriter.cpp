#include "portwriter.h"

PortWriter::PortWriter(QObject *parent, QSerialPort *port, CSOK_DBase *DBase, QMutex *mutexDB):
    QThread(parent)
{
    this->m_port = port;
    this->DBase = DBase;
    this->mutexDB = mutexDB;
    this->mutexQueue = new QMutex();
    qDebug() << "PortWriter is created";
}

void PortWriter::run()
{
    while (true){
        mutexQueue->lock();
        if (!queueOfMessages.isEmpty()) {

            QByteArray frame;
            frame = queueOfMessages.dequeue();
            mutexQueue->unlock();
            decodeInputFrame(frame);
            frame.clear();
        } else {
            mutexQueue->unlock();
        }
        dispatchOnline();
        usleep(100000);
    }
}

//Определяем тип входящего сообщения
void PortWriter::decodeInputFrame(QByteArray inputFrame)
{
    QString str = XBConvert::addressToString(inputFrame);
    qDebug() << "run " + str;
    //XBInfo::addMessage(str, false);

    if (inputFrame[0] != 0x7E){
        return;
    }

    //Проверка контрольной суммы
    int checkSumm = 0;
    for (int i = 3; i < inputFrame.length(); i++){
        checkSumm += inputFrame[i];
    }

    if (((byte) checkSumm) != 0xFF){
        qDebug() << "checkSumm error";
        return;
    }

    if (inputFrame[3] != API_RX_Packet){
        return;
    }

    //Определяем серийный номер устройства пославшего запрос
    QByteArray address;
    address.resize(8);
    qCopy(inputFrame.begin() + 4, inputFrame.begin() + 12, address.begin());

    QString addressStr = XBConvert::addressToString(address);
    bool changeResult = false;
    int result = 0;
    byte code = 0x00;

    //Определяем тип фрейма
    switch (0xFF & inputFrame[inputFrame.length() - 2]){
        case REQ_REGISTRATION: //Запрос на регистрацию
        {
            qDebug() << "REQ_REGISTRATION " + addressStr;

            //Поиск устройства в БД
            mutexDB->lock();
            int n = DBase->findSN(addressStr); //Определяем номер таблицы
            mutexDB->unlock();

            if (n != -1) { //Устройство зарегистрировано
                mutexDB->lock();
                QList <quint8> idNum = DBase->getID(n, addressStr);
                mutexDB->unlock();

                removeDevAddr(addressStr);
                switch (n){
                    case 1:  //Рабочий в сети
                    {
                        sendAnswer(ACK_REGISTRATION, address, 20, (QList <byte>() << 0));
                        WorkerOnLine* worker = getWorkerOnlineByAddress(address);
                        if (worker != NULL) {
                            removeWorkerOnline(worker);
                        }
                        mutexDB->lock();
                        worker = DBase->getWorker(addressStr);
                        mutexDB->unlock();
                        if (worker != NULL) {
                            addWorker(worker);
                            //Проверяем разрядку
                            if (inputFrame[inputFrame.length() - 3] == 0x00) {
                                worker->setCharged(true);
                            } else {
                                worker->setCharged(false);
                            }
                        }

                        break;
                    }

                    case 2:  //Контролер в сети
                    {
                        sendAnswer(ACK_REGISTRATION, address, 20, (QList<byte>() << idNum[1]));
                        ControllerOnLine *controllerOnline = getControllerOnlineByAddress(address);
                        if (controllerOnline == NULL) {
                            QString date = QDate::currentDate().toString("dd.MM.yyyy");
                            QString timeOn = QTime::currentTime().toString("hh:mm:ss");
                            //Добавляем в БД
                            mutexDB->lock();
                            controllerOnline = DBase->getControler(addressStr);
                            DBase->addControllerWork(date, idNum[1], timeOn);
                            mutexDB->unlock();

                            controllerOnline->setDate(date);
                            controllerOnline->setTimeOn(timeOn);
                            addController(controllerOnline);
                        } else if (controllerOnline->isCall() &&
                                   (controllerOnline->getCallingID().isEmpty() || controllerOnline->getCallingID().isNull())) {
                            controllerOnline->setCall(false);
                            groupController.remove(controllerOnline->getGroup());
                            emit updateControllerBusy(controllerOnline, false);
                        }

                        RequestService* req = getRequestByControllerId(controllerOnline->getID());
                        if (req != NULL) {
                            QString idWorker = req->getWorkerID();
                            removeRequest(req, 1);
                            groupController.remove(controllerOnline->getGroup());
                            WorkerOnLine *worker = getWorkerById(idWorker);
                            if (worker != NULL) {
                                moveWorkerToQueue(worker, true);
                            }
                        }

                        unQueueWorker(controllerOnline);

                        if (inputFrame[inputFrame.length() - 3] == 0x00) {
                            controllerOnline->setCharge(true);
                        } else {
                            controllerOnline->setCharge(false);
                        }

                        break;
                    }
                }
            } else {
                //Устройство не зарегистрировано. Приглашение на регистрацию
                if (!reqRegAddr.contains(addressStr)){
                    reqRegAddr.append(addressStr);
                    sendAnswer(POWER_OFF_SEND, address, 20, (QList <byte>() << 0));
                    emit reqNewDevice(addressStr);
                }
            }

            emit chargeUpdate(controllersOnline, workersOnline);
            break;
        }

        //Вызов
        case REQ_SERVICE:
        {
            qDebug() << "REQ_SERVICE " + addressStr;
            WorkerOnLine* worker = getWorkerOnlineByAddress(address);
            if (worker != NULL) {
                sendAnswer(ACK_SERVICE_HOST, address, 20, (QList <byte>() << 0));
                qDebug() << "ACK_SERVICE_HOST " + addressStr + "(id:" +worker->getID() + ")";

                //Фиксируем время принятия запроса
                QString date = QDate::currentDate().toString("dd.MM.yyyy");
                QString time = QTime::currentTime().toString("hh:mm:ss");

                RequestService* request = getRequestByWorkerId(worker->getID());
                //Запрос не выполнен
                if (request != NULL) {
                    //Определяем статус запроса
                    switch (request->getStatus()) {
                        case HARDWARE_ACCEPT_WAIT:
                        {
                            removeRequest(request, 1);
                            //Формируем новый запрос
                            sendReq(worker, date, time);
                            break;
                        }
                        case START_WAIT:
                            checkReq(worker, request, date, time);
                            break;
                        case END_WAIT:
                            checkReq2(worker, request, date, time);
                            break;
                    }
                } else {
                    //Формируем новый запрос

                    sendReq(worker, date, time);
                }

                if (worker != NULL) {
                    if (inputFrame[inputFrame.length() - 3] == 0x00) {
                        worker->setCharged(true);
                    } else {
                        worker->setCharged(false);
                    }
                }

                emit chargeUpdate(controllersOnline, workersOnline);
            }

            break;
        }

        case ACK_HARDWARE_CALL:
        {
            qDebug() << "ACK_HARDWARE_CALL from " + addressStr;

            ControllerOnLine* controller = getControllerOnlineByAddress(address);
            QString idWorker = controller->getCallingID();

            //QStringList lst = idWorker.split(".");
            //byte idToSend = ((lst.at(0).toInt() << 4) & 0xF0) | (lst.at(1).toInt() & 0x0F);

            if (!idWorker.isEmpty()) {
                sendAnswer(ACK_ACK, address, 21, idToList(idWorker));
            }

            if (controller != NULL) {
                RequestService *request = getRequestByWorkerId(idWorker);
                if (request != NULL) {
                    QString time = QTime::currentTime().toString("hh:mm:ss");
                    request->setHardwareAcceptTime(time);
                    //request->setControllerID(controller->getID());
                    request->setStatus(START_WAIT);

                    mutexDB->lock();
                    DBase->changeHardwareAnswerReqTime(request->getDate(),
                                                       request->getRequestTime(),
                                                       time);
                    mutexDB->unlock();

                    RequestService* req = new RequestService();
                    req->cloneFrom(request);
                    emit (updateRequest(req, false));

                } else {
                    WorkerOnLine* worker = getWorkerById(idWorker);

                    if (worker != NULL) {
                        sendAnswer(POWER_OFF_SEND, worker->getSerialNumber(), 20, (QList<byte>() << 0));
                        removeWorkerOnline(worker->getID());
                        removeWorkerOnline(idWorker);
                    }
                    //sendAnswer(REQ_ESCAPE, controller->getSerialNumber(), 20, (QList<byte>() << controller->getID()));
                    //controller->setCall(false);
                    //controller->setBusy(false);
                    //controller->setCallingID("");
                    //controller->setService("");
                    //ControllerOnLine* cntrl = new ControllerOnLine();
                    //cntrl->cloneFrom(controller);
                    //emit(updateControllerBusy(cntrl, false));
                }
                if (inputFrame[inputFrame.length() - 5] == 0x00) {
                    controller->setCharge(true);
                } else {
                    controller->setCharge(false);
                }

                emit chargeUpdate(controllersOnline, workersOnline);
            } else {
                sendAnswer(REQ_ESCAPE, address, 20, (QList<byte>() << 0));
            }

            break;
        }

        case K_REQ_SERVICE:
        {
            qDebug() << "K_REQ_SERVICE from " + addressStr;
            byte receiveGroup = inputFrame.at(inputFrame.length() - 3);
            byte receiveNum = inputFrame.at(inputFrame.length() - 4);

            QString workerID = getId(receiveGroup, receiveNum);

            WorkerOnLine *worker = getWorkerById(workerID);
            ControllerOnLine* controller = getControllerOnlineByAddress(address);

            sendAnswer(ACK_ACK, address, 21, idToList(workerID));

            if (worker == NULL && controller != NULL) {
                sendAnswer(REQ_ESCAPE, address, 21, idToList(workerID));
                if (inputFrame[inputFrame.length() - 4] == 0x00) {
                    controller->setCharge(true);
                } else {
                    controller->setCharge(false);
                }

                controller->setBusy(false);
                controller->setCall(false);
                controller->setCallingID(0);
                controller->setService(0);
                ControllerOnLine* cntrl = new ControllerOnLine();
                cntrl->cloneFrom(controller);
                emit(updateControllerBusy(cntrl, false));

            } else if (worker != NULL && controller == NULL) {
                sendAnswer(REQ_ESCAPE, address, 20, (QList<byte>() << 0));
                sendAnswer(POWER_OFF_SEND, worker->getSerialNumber(), 20, (QList<byte>() << 0));
                //sendAnswer(REQ_END_SERVICE_OK, worker->getSerialNumber(), 20,worker->getID());

                RequestService *request = getRequestByWorkerId(workerID);

                removeRequest(request, 1);
                removeWorkerOnline(worker->getID());

            } else if (worker != NULL && controller != NULL) {

                RequestService *request = getRequestByWorkerId(workerID);
                QString time = QTime::currentTime().toString("hh:mm:ss");

                if (request != NULL) {
                    sendAnswer(K_REQ_SERVICE, worker->getSerialNumber(), 20, (QList<byte>() << 0));
                    request->setStartTime(time);
                    request->setStatus(END_WAIT);

                    mutexDB->lock();
                    DBase->changeAcceptReqTime(request->getDate(), request->getRequestTime(), time);
                    mutexDB->unlock();

                    RequestService* req = new RequestService();
                    req->cloneFrom(request);
                    emit (updateRequest(req, false));

                    controller->setBusy(true);
                    controller->setCall(false);
                    controller->setService(workerID);

                    ControllerOnLine* cntrl = new ControllerOnLine();
                    cntrl->cloneFrom(controller);
                    emit(updateControllerBusy(cntrl, false));
                } else {
                    sendAnswer(REQ_ESCAPE, address, 20, (QList<byte>() << 0));
                    controller->setBusy(false);
                    controller->setCall(false);
                    controller->setCallingID(0);
                    controller->setService(0);
                    ControllerOnLine* cntrl = new ControllerOnLine();
                    cntrl->cloneFrom(controller);
                    emit(updateControllerBusy(cntrl, false));

                    sendAnswer(POWER_OFF_SEND, worker->getSerialNumber(), 20, (QList<byte>() << 0));
                    removeWorkerOnline(worker->getID());
                }

                if (inputFrame[inputFrame.length() - 5] == 0x00) {
                    controller->setCharge(true);
                } else {
                    controller->setCharge(false);
                }

                emit chargeUpdate(controllersOnline, workersOnline);
            }

            break;
        }

        case REQ_END_SERVICE_NO:
            qDebug() << "REQ_END_SERVICE_NO from " + addressStr;
            changeResult = true;
            code = REQ_END_SERVICE_NO;
            result = 2;

        case REQ_END_SERVICE_OK:
        {
            if (!changeResult) {
                qDebug() << "REQ_END_SERVICE from " + addressStr;
                result = 1;
                code = REQ_END_SERVICE_OK;
            }
            ControllerOnLine* controller = getControllerOnlineByAddress(address);
            QString workerID;
            workerID = "0.0";
            if (controller != NULL && !controller->getCallingID().isEmpty()) {
                workerID = controller->getCallingID();
            }
            //byte receiveGroup = inputFrame.at(inputFrame.length() - 3);
            //byte receiveNum = inputFrame.at(inputFrame.length() - 4);

            //QString workerID = QString::number(receiveGroup) + "." + QString::number(receiveNum);
            sendAnswer(ACK_SERVICE_HOST, address, 21, idToList(workerID));

            WorkerOnLine *worker = getWorkerById(workerID);
            if (worker != NULL) {
                sendAnswer(code, worker->getSerialNumber(), 20, (QList<byte>() << 0));
                removeWorkerOnline(workerID);
            }

            RequestService *request = getRequestByWorkerId(workerID);

            if (request != NULL) {
                request->setResult(result);
                removeRequest(request, 0);
            }

            if (controller != NULL){
                controller->setBusy(false);
                controller->setCall(false);
                controller->addWorker(workerID);
                controller->setService("");
                controller->setCallingID("");

                moveControllerToEnd(controller);

                ControllerOnLine* cntrl = new ControllerOnLine();
                cntrl->cloneFrom(controller);
                emit(updateControllerBusy(cntrl, false));

                if (inputFrame[inputFrame.length() - 5] == 0x00) {
                    controller->setCharge(true);
                } else {
                    controller->setCharge(false);
                }

                groupController.remove(controller->getGroup());
                unQueueWorker(controller);
                emit chargeUpdate(controllersOnline, workersOnline);
            } else {
                sendAnswer(REQ_ESCAPE, address, 20, (QList<byte>() << 0));
            }

            break;
        }

        case REQ_ESCAPE:
        {
            qDebug() << "REQ_ESCAPE from " + addressStr;
            QString workerID = getWorkerOnlineID(address);
            sendAnswer(ACK_SERVICE_HOST, address, 20, (QList<byte>() << 0));

            QString controllerID;
            RequestService *request = getRequestByWorkerId(workerID);
            if (request != NULL) {
                controllerID = request->getControllerID();
            }
            removeRequest(request, 3);

            if (!workerID.isEmpty() && !workerID.isNull()){
                removeWorkerOnline(workerID);
                ControllerOnLine *controller = getControllerById(controllerID);
                if (controller != NULL) {
                    controller->setBusy(false);
                    controller->setCall(false);

                    groupController.remove(controller->getGroup());
                    controller->setGroup(0);
                    controller->setService("");
                    controller->setCallingID("");
                    moveControllerToEnd(controller);

                    sendAnswer(REQ_ESCAPE, controller->getSerialNumber(), 20, (QList<byte>() << controller->getID().toInt()));
                    ControllerOnLine* cntrl = new ControllerOnLine();
                    cntrl->cloneFrom(controller);
                    emit(updateControllerBusy(cntrl, false));
                    unQueueWorker(controller);
                }
            }
            break;
        }

        case GET_ZPRSF:
        {
            qDebug() << "GET_ZPRSF from " + addressStr;
            ControllerOnLine* controller = getControllerOnlineByAddress(address);
            if (controller != NULL) {
                controller->setBusy(false);
                controller->setCall(true);

                if (!controller->getCallingID().isEmpty() && !controller->getCallingID().isNull()) {
                    WorkerOnLine* worker = getWorkerById(controller->getCallingID());
                    if (worker != NULL) {
                        moveWorkerToQueue(worker, true);
                        RequestService* request = getRequestByWorkerId(worker->getID());
                        if (request != NULL) {
                            removeRequest(request, 3);
                        }
                    }
                }

                ControllerOnLine* cntrl = new ControllerOnLine();
                cntrl->cloneFrom(controller);
                emit updateControllerBusy(cntrl, false);
            }
            break;
        }

        case CANCEL_SF:
        {
            qDebug() << "CANCEL_SF from " + addressStr;
            ControllerOnLine* controller = getControllerOnlineByAddress(address);
            if (controller != NULL) {
                controller->setBusy(false);
                controller->setCall(false);

                groupController.remove(controller->getGroup());
                unQueueWorker(controller);

                ControllerOnLine* cntrl = new ControllerOnLine();
                cntrl->cloneFrom(controller);
                emit updateControllerBusy(cntrl, false);
            }
            break;
        }

        case GET_ZPRNUM:
        {
            qDebug() << "GET_ZPRNUM from " + addressStr;
            byte receiveGroup = inputFrame.at(inputFrame.length() - 4);
            byte receiveNum = inputFrame.at(inputFrame.length() - 3);

            ControllerOnLine* controller;

            QString workerID = getId(receiveGroup, receiveNum);
            mutexDB->lock();
            QString workerFIO = DBase->getWorkerFIO(workerID);
            mutexDB->unlock();

            if (!workerFIO.isNull()) {
                sendAnswer(ZPR_EXIST, address, 20, (QList<byte>() << 0x00));
                QString date = QDate::currentDate().toString("dd.MM.yyyy");
                QString time = QTime::currentTime().toString("hh:mm:ss");

                WorkerOnLine* worker = getWorkerById(workerID);
                if (worker != NULL) {
                    sendAnswer(K_REQ_SERVICE, worker->getSerialNumber(), 20, (QList<byte>() << 0));
                }

                RequestService* request = getRequestByWorkerId(workerID);
                if (request != NULL) {
                    controller = getControllerById(request->getControllerID());
                    if (controller != NULL && controller->getSerialNumber() != address) {
                        controller->setCallingID("");
                        controller->setService("");
                        controller->setBusy(false);
                        controller->setCall(false);
                        groupController.remove(controller->getGroup());

                        sendAnswer(REQ_ESCAPE, controller->getSerialNumber(), 20, (QList<byte>() << controller->getID().toInt()));
                        removeRequest(request, 3);

                        ControllerOnLine* cntrl = new ControllerOnLine();
                        cntrl->cloneFrom(controller);
                        emit updateControllerBusy(cntrl, false);
                        unQueueWorker(controller);
                    }
                }
                request = new RequestService(workerID, date, time);
                controller = getControllerOnlineByAddress(address);
                if (controller != NULL) {
                    request->setCalledControllerID(controller->getID());
                    request->setControllerID(controller->getID());
                    request->setControllerFIO(controller->getFIO());
                    controller->setService(workerID);
                    controller->setCallingID(workerID);
                    controller->setBusy(true);
                    controller->setCall(false);

                    ControllerOnLine* cntrl = new ControllerOnLine();
                    cntrl->cloneFrom(controller);
                    emit updateControllerBusy(cntrl, false);
                }
                request->setStatus(END_WAIT);
                request->setWorkerFIO(workerFIO);
                request->setStartTime(time);
                request->setHardwareAcceptTime(time);

                addRequest(request);
                mutexDB->lock();
                DBase->changeRequest(request);
                mutexDB->unlock();

            } else {
                sendAnswer(ZPR_NOTEXIST, address, 20, (QList<byte>() << 0x00));
                controller = getControllerOnlineByAddress(address);
                if (controller != NULL) {
                    controller->setService("");
                    controller->setCallingID("");
                    controller->setBusy(false);
                    controller->setCall(false);
                    groupController.remove(controller->getGroup());
                    ControllerOnLine* cntrl = new ControllerOnLine();
                    cntrl->cloneFrom(controller);
                    emit updateControllerBusy(cntrl, false);
                    //sendAnswer(K_REQ_SERVICE, address, 20, (QList<byte>() << 0));
                    usleep(2500000);
                    unQueueWorker(controller);
                }
            }
            break;
        }
/*
        case REQ_STATUS:
        {
            qDebug() << "REQ_STATUS from " + addressStr;

            WorkerOnLine *worker = getWorkerOnlineByAddress(address);
            if (worker != NULL) {
                sendAnswer(ACK_STATUS, address, 20, 0);
                if (inputFrame[inputFrame.length() - 4] == 0x00) {
                    worker->setCharged(true);
                } else {
                    worker->setCharged(false);
                }
                chargeUpdate(controllersOnline, workersOnline);
            } else {
                sendAnswer(ACK_SERVICE_HOST, address, 20, 0);
                sendAnswer(K_REQ_SERVICE, address, 20, 0);
                sendAnswer(REQ_END_SERVICE_OK, address, 20, 0);

            }
            break;
        }
*/
        case POWER_OFF_GET:
        {
            qDebug() << "POWER_OFF from " + addressStr;

            ControllerOnLine* controller = getControllerOnlineByAddress(address);
            if (controller != NULL){
                controller->setTimeOff(QTime::currentTime().toString("hh:mm:ss"));
                controller->setService(0);
                controllersOnline.remove(controllersOnline.indexOf(controller));
                mutexDB->lock();
                DBase->changeControllerTimeOff(controller->getDate(), controller->getTimeOn(), controller->getTimeOff());
                mutexDB->unlock();

                emit(updateControllerBusy(controller, false));
            }
            break;
        }

        case UNCHARGED:
        {
            qDebug() << "UNCHARGED " + addressStr;
            int n = DBase->findSN(addressStr); //Определяем номер таблицы

            if (n != -1) { //Устройство зарегистрировано

                switch (n){
                    case 1:  //Батарея запросника разряжена
                    {
                        WorkerOnLine* worker = getWorkerOnlineByAddress(address);
                        if (worker != NULL){
                            worker->setCharged(false);
                        }
                        break;
                    }

                    case 2:  //Батарея пейджера разряжена
                    {
                        ControllerOnLine *controllerOnline = getControllerOnlineByAddress(address);
                        if (controllerOnline != NULL){
                            controllerOnline->setCharge(false);
                        }

                    }
                }
            emit chargeUpdate(controllersOnline, workersOnline);
            }
            break;
        }
/*
        case ACK_ONLINE:
        {
            qDebug() << "ACK_ONLINE from " + addressStr;

            mutexDB->lock();
            int n = DBase->findSN(addressStr); //Определяем номер таблицы
            mutexDB->unlock();
            if (n != -1) { //Устройство зарегистрировано
                mutexDB->lock();
                mutexDB->unlock();

                switch (n){
                    case 1:
                    {
                        WorkerOnLine* worker = getWorkerOnlineByAddress(address);
                        if (worker != NULL){
                            if (inputFrame[inputFrame.length() - 4] == 0x00) {
                                worker->setCharged(true);
                            } else {
                                worker->setCharged(false);
                            }

                            worker->setSendStatus(false);
                        }
                        break;
                    }

                    case 2:
                    {
                        ControllerOnLine *controllerOnline = getControllerOnlineByAddress(address);
                        if (controllerOnline != NULL){
                            if (inputFrame[inputFrame.length() - 4] == 0x00) {
                                controllerOnline->setCharge(true);
                            } else {
                                controllerOnline->setCharge(false);
                            }
                            controllerOnline->setSendStatus(false);
                        }
                    }
                }
            emit chargeUpdate(controllersOnline, workersOnline);
            }
            break;
        }*/
    }
}


ControllerOnLine* PortWriter::getControllerById(QString id)
{
    for (int i = 0; i < controllersOnline.count(); i++){
        if (controllersOnline.at(i)->getID() == id){
            return controllersOnline.at(i);
        }
    }

    return NULL;
}

void PortWriter::moveWorkerToQueue(WorkerOnLine* worker, bool first)
{
    for (int i = queueByGroups.count() - 1; i >= 0; i--) {

        QMap <qint8, QQueue <WorkerOnLine*> > mapGroup = queueByGroups.at(i);
        if (mapGroup.contains(worker->getGroupNum())) {
            QQueue <WorkerOnLine*> queue = mapGroup[worker->getGroupNum()];
            if (!queue.contains(worker)) {
                if (first) {
                    queue.insert(queue.begin(), worker);
                } else {
                    queue.enqueue(worker);
                }

                mapGroup[worker->getGroupNum()] = queue;
                queueByGroups.replace(i, mapGroup);
                return;
            } else {
                return;
            }
        }
    }

    QQueue< WorkerOnLine* > queue;
    if (first) {
        queue.insert(queue.begin(), worker);
    } else {
        queue.enqueue(worker);
    }

    QMap <qint8, QQueue <WorkerOnLine*> > workersMap;
    workersMap[worker->getGroupNum()] = queue;
    queueByGroups.enqueue(workersMap);
}

void PortWriter::unQueueWorker(ControllerOnLine* freeController)
{
    if (freeController != NULL){
        int group = freeController->getGroup();
        WorkerOnLine* workerOnline = getWorkerByGroup(group);

        if (workerOnline == NULL) {
            freeController->setGroup(0);
            bool found = false;
            QList<qint32> list = groupController.keys();
            if (list.count() == 0) {
                found = true;
            }

            for (int i = 0; i < list.count(); i++) {
                if (groupController[list.at(i)] != group) {
                    found = true;
                    break;
                }
            }

            if (found) {
                for (int i = queueByGroups.count() - 1; i >= 0; i--) {
                    if (!((QMap <qint8, QQueue <WorkerOnLine*> >)queueByGroups.at(i)).contains(group)) {
                        QMap <qint8, QQueue <WorkerOnLine*> > map = queueByGroups.at(i);
                        qint32 keyGroup = map.keys().at(0);
                        if (!groupController.contains(keyGroup)) {
                            QQueue <WorkerOnLine*> queue = map[keyGroup];
                            if (!queue.isEmpty()) {
                                workerOnline = queue.dequeue();
                                group = workerOnline->getGroupNum();
                                if (queue.isEmpty()) {
                                    ((QMap <qint8, QQueue <WorkerOnLine*> >)queueByGroups.at(i)).remove(group);
                                    queueByGroups.removeAt(i);
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (workerOnline != NULL) {
            usleep(3000000); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            qDebug() << "send REQ_SERVICE to " +
                        freeController->getSerialNumberStr() +
                        " from worker " +
                        workerOnline->getID();

            freeController->setGroup(group);
            freeController->setBusy(false);
            freeController->setCall(true);
            freeController->setCallingID(workerOnline->getID());
            if (!groupController.contains(group)) {
                groupController[group] = freeController->getID().toInt();
            }

            sendAnswer(REQ_SERVICE, freeController->getSerialNumber(), 21, idToList(workerOnline->getID()));

            ControllerOnLine* cntrl = new ControllerOnLine();
            cntrl->cloneFrom(freeController);
            emit(updateControllerBusy(cntrl, false));

            QString date = QDate::currentDate().toString("dd.MM.yyyy");
            QString time = QTime::currentTime().toString("hh:mm:ss");
            RequestService* request = getRequestByWorkerId(workerOnline->getID());

            if (request != NULL){
                removeRequest(request, 4);
            }

            request = new RequestService(workerOnline->getID(), date, time);
            request->setControllerFIO(freeController->getFIO());
            request->setCalledControllerID(freeController->getID());
            request->setControllerID(freeController->getID());
            request->setStatus(START_WAIT);
            request->setWorkerFIO(workerOnline->getFIO());
            request->setHardwareAcceptTime(request->getRequestTime());
            request->setStartTime(request->getRequestTime());
            addRequest(request);
            mutexDB->lock();
            DBase->changeRequest(request);
            mutexDB->unlock();
        }
    }
}

void PortWriter::removeWorkerOnline(WorkerOnLine* worker)
{
    for (int i = queueByGroups.count() - 1; i >= 0; i--) {
        QMap <qint8, QQueue <WorkerOnLine*> > groupsMap = queueByGroups.at(i);
        if (groupsMap.contains(worker->getGroupNum())) {
            QQueue <WorkerOnLine*> queue = groupsMap[worker->getGroupNum()];
            if (!queue.isEmpty()) {
                for (int j = queue.count() - 1; j >= 0; j--) {
                    if (((WorkerOnLine*)queue.at(j))->getID() == worker->getID()) {
                        queue.removeAt(j);
                        groupsMap[worker->getGroupNum()] = queue;
                        queueByGroups.replace(i, groupsMap);
                        break;
                    }
                }

                if (queue.isEmpty()) {
                    groupsMap.remove(worker->getGroupNum());
                    queueByGroups.removeAt(i);
                }
            }
            break;
        }
    }
    workersOnline.remove(workersOnline.indexOf(worker));
    delete worker;
}

void PortWriter::removeWorkerOnline(QString id)
{
    foreach (WorkerOnLine* worker, workersOnline) {
        if (worker->getID() == id) {
            removeWorkerOnline(worker);
            break;
        }
    }
}

void PortWriter::removeControllerOnline(ControllerOnLine* controller)
{
    if (controllersOnline.contains(controller)) {
        ControllerOnLine* cntrl = new ControllerOnLine();
        cntrl->cloneFrom(controller);
        cntrl->setTimeOff(QTime::currentTime().toString("hh:mm:ss"));
        mutexDB->lock();
        DBase->changeControllerTimeOff(cntrl->getDate(), cntrl->getTimeOn(), cntrl->getTimeOff());
        mutexDB->unlock();
        emit(updateControllerBusy(cntrl, false));
        controllersOnline.remove(controllersOnline.indexOf(controller));
        delete controller;
    }
}

void PortWriter::moveControllerToEnd(ControllerOnLine* controller)
{
    if (controllersOnline.contains(controller)) {
        controllersOnline.remove(controllersOnline.indexOf(controller));
        controllersOnline.append(controller);
    }
}

WorkerOnLine* PortWriter::getWorkerById(QString id)
{ 
    foreach (WorkerOnLine* worker, workersOnline) {
        if (worker->getID() == id) {
            return worker;
        }
    }

    return NULL;
}

RequestService* PortWriter::getRequestByWorkerId(QString id){

    foreach (RequestService* request, requests) {
        if (request->getWorkerID() == id) {
            return request;
        }
    }

    return NULL;
}

RequestService* PortWriter::getRequestByControllerId(QString id)
{
    foreach (RequestService* request, requests) {
        if (request->getCalledControllerID() == id || request->getControllerID() == id) {
            return request;
        }
    }

    return NULL;
}

void PortWriter::addWorker(WorkerOnLine* worker)
{
    if (worker != NULL) {
        workersOnline.append(worker);
    }
}

void PortWriter::addController(ControllerOnLine* controller)
{
    if (controller != NULL) {
        controllersOnline.append(controller);
        ControllerOnLine* cntrl = new ControllerOnLine();
        cntrl->cloneFrom(controller);
        emit(updateControllerBusy(cntrl, true));
    }
}

void PortWriter::addRequest(RequestService* request)
{
    if (request != NULL) {
        requests.append(request);

        RequestService* req = new RequestService();
        req->cloneFrom(request);
        emit (updateRequest(req, true));

        mutexDB->lock();
        DBase->addRequest(request->getDate(),
                          request->getWorkerID(),
                          request->getRequestTime());
        DBase->changeControllerIDReq(request->getDate(), request->getRequestTime(), request->getControllerID().toInt());
        mutexDB->unlock();
    }
}

QString PortWriter::getWorkerOnlineID(QByteArray serialNumber)
{
    foreach (WorkerOnLine* worker, workersOnline) {
        if (worker->getSerialNumber() == serialNumber) {
            return worker->getID();
        }
    }
    return "";
}

WorkerOnLine* PortWriter::getWorkerByGroup(int group)
{
    for (int i = queueByGroups.count() - 1; i >= 0; i--) {
        QMap <qint8, QQueue <WorkerOnLine*> > groupMap = queueByGroups.at(i);
        if (groupMap.contains(group)) {
            QQueue <WorkerOnLine*> queue = groupMap[group];
            if (!queue.isEmpty()) {
                WorkerOnLine* worker = queue.dequeue();
                groupMap[group] = queue;
                queueByGroups.replace(i, groupMap);
                if (queue.isEmpty()) {
                    groupMap.remove(group);
                    queueByGroups.removeAt(i);
                }
                return worker;
            }
            break;
        }
    }
    return NULL;
}



//Вычисление контрольной суммы посылаемого API фрейма
byte PortWriter::CheckSumm_Calc(byte* data, int length)
{
    int chkS = 0;
    for (int i = 3; i < length - 1; i++)
    {
        chkS += (byte) data[i];
    }

    return (0xFF - (byte)chkS);
}

WorkerOnLine* PortWriter::getWorkerOnlineByAddress(QByteArray serialNumber)
{
    foreach (WorkerOnLine *worker, workersOnline) {
        if (worker->getSerialNumber() == serialNumber){
            return worker;
        }
    }
    return  NULL;
}

ControllerOnLine* PortWriter::getControllerOnlineByAddress(QByteArray serialNumber)
{
    foreach (ControllerOnLine* controller, controllersOnline) {
        if (controller->getSerialNumber() == serialNumber) {
            return controller;
        }
    }
    return  NULL;
}

ControllerOnLine* PortWriter::getFreeController(int groupOfNewWorker)
{
    if (!groupController.contains(groupOfNewWorker)) {
        foreach (ControllerOnLine* controller, controllersOnline) {
            if (!controller->isBusy() && !controller->isCall() && controller->getGroup() != groupOfNewWorker) {
                return controller;
            }
        }
    }
    return NULL;
}

QList <byte> PortWriter::idToList(QString id)
{
    QStringList lst = id.split(".");
    QList <byte> idToSend;

    if (!lst.isEmpty()) {
        idToSend.append(lst.at(1).toInt());
        idToSend.append(lst.at(0).toInt());
    }

    return idToSend;
}

QString PortWriter::getId(quint8 group, quint8 id)
{
    return QString::number(group) + "." + QString::number(id);
}

void PortWriter::dispatchOnline()
{
    QTime currentTime = QTime::currentTime();
    QString currentDate = QDate::currentDate().toString("dd.MM.yyyy");

    foreach (RequestService* request, requests) {

        WorkerOnLine* worker = getWorkerById(request->getWorkerID());
        ControllerOnLine* controller = getControllerById(request->getCalledControllerID());

        if (worker != NULL && controller != NULL) {
            if (request->getStatus() == START_WAIT) {
                if (QTime::fromString(request->getRequestTime(), "hh:mm:ss").secsTo(currentTime) > START_TIME_WAIT_SEC){
                    request->setStartTime("Нет ответа");
                    request->setEndTime("Нет ответа");

                    mutexDB->lock();
                    DBase->changeAcceptReqTime(request->getDate(), request->getRequestTime(), request->getStartTime());
                    DBase->changeEndReqTime(request->getDate(), request->getRequestTime(), request->getEndTime(), 0);
                    mutexDB->unlock();
                    requests.remove(requests.indexOf(request));
                    emit (updateRequest(request, false));
                   // emit(updateRequests(requests));
                    //unQueueWorker(controller);

                    groupController.remove(controller->getGroup());
                    if (controllersOnline.count() > 1) {
                        sendReq(worker, currentDate, currentTime.toString("hh:mm:ss"));
                    } else {
                        removeAllWorkers();
                    }

                    controller->setBusy(false);
                    controller->setCall(false);
                    controller->setService(0);
                    controller->setCallingID(0);


                    controller->setGroup(0);

                    //Перемещаем в конец списка, чтобы в случае занятости не попасть на него, если в сети несколько контроллеров
                    moveControllerToEnd(controller);

                    sendAnswer(REQ_ESCAPE, controller->getSerialNumber(), 20, (QList<byte>() << controller->getID().toInt()));

                    ControllerOnLine* cntrl = new ControllerOnLine();
                    cntrl->cloneFrom(controller);
                    emit(updateControllerBusy(cntrl, false));
                }
            } else if (request->getStatus() == END_WAIT) {
                if (QTime::fromString(request->getStartTime(), "hh:mm:ss").secsTo(currentTime) > END_TIME_WAIT_SEC) {

                    request->setEndTime(currentTime.toString("hh:mm:ss"));
                    groupController.remove(controller->getGroup());
                    controller->setBusy(false);
                    controller->setCall(false);
                    controller->setService(0);
                    controller->setCallingID(0);
                    controller->setGroup(0);

                    sendAnswer(REQ_ESCAPE, controller->getSerialNumber(), 20, (QList<byte>() << controller->getID().toInt()));

                    ControllerOnLine* cntrl = new ControllerOnLine();
                    cntrl->cloneFrom(controller);
                    emit(updateControllerBusy(cntrl, false));

                    sendAnswer(REQ_END_SERVICE_OK, worker->getSerialNumber(), 20, (QList<byte>() << 0));

                    controllersOnline.remove(controllersOnline.indexOf(controller));
                    controllersOnline.append(controller);

                    mutexDB->lock();
                    DBase->changeEndReqTime(request->getDate(), request->getRequestTime(), request->getEndTime(), 0);
                    mutexDB->unlock();
                    requests.remove(requests.indexOf(request));
                    emit (updateRequest(request, false));
                    workersOnline.remove(workersOnline.indexOf(worker));
                    unQueueWorker(controller);
                }
            }
        } else if (worker == NULL && controller != NULL) {
            /*sendAnswer(REQ_ESCAPE, controller->getSerialNumber(), 20, (QList<byte>() << controller->getID()));
            controller->setBusy(false);
            controller->setCall(false);
            controller->setService(0);
            controller->setCallingID(0);
            ControllerOnLine* cntrl = new ControllerOnLine();
            cntrl->cloneFrom(controller);
            emit(updateControllerBusy(cntrl, false));

            request->setEndTime("Отмена");
            request->setStartTime("Отмена");

            mutexDB->lock();
            DBase->changeAcceptReqTime(request->getDate(), request->getRequestTime(), "Отмена");
            DBase->changeEndReqTime(request->getDate(), request->getRequestTime(), "Отмена", 0);
            mutexDB->unlock();

            requests.remove(requests.indexOf(request));
            emit (updateRequest(request, false));*/

        } else if (worker != NULL && controller == NULL) {
            switch (request->getStatus()) {
                case HARDWARE_ACCEPT_WAIT:
                {
                    removeRequest(request, 1);
                    //Формируем новый запрос
                    sendReq(worker, currentDate, currentTime.toString("hh:mm:ss"));
                    break;
                }
                case START_WAIT:
                    checkReq(worker, request, currentDate, currentTime.toString("hh:mm:ss"));
                    break;
                case END_WAIT:
                    checkReq2(worker, request, currentDate, currentTime.toString("hh:mm:ss"));
                    break;
            }
        } else {
            request->setEndTime("Сбой");
            mutexDB->lock();
            DBase->changeEndReqTime(request->getDate(), request->getRequestTime(), "Сбой", 0);
            mutexDB->unlock();
            requests.remove(requests.indexOf(request));
            emit (updateRequest(request, false));
        }
    }
}

void PortWriter::removeAllWorkers()
{
    queueByGroups.clear();
    foreach (WorkerOnLine* worker, workersOnline) {
        sendAnswer(POWER_OFF_SEND, worker->getSerialNumber(), 20, QList<quint8>() << 0);
    }

    qDeleteAll(workersOnline);
    workersOnline.clear();
}

void PortWriter::sendReq(WorkerOnLine* worker, QString date, QString time)
{
    QByteArray address = worker->getSerialNumber();
    QString addressStr = XBConvert::addressToString(address);

    ControllerOnLine* controller = getFreeController(worker->getGroupNum());

    //Найден свободный контроллер
    if (controller != NULL) {
        groupController[worker->getGroupNum()] = controller->getID().toInt();
        controller->setBusy(false);
        controller->setCall(true);
        controller->setCallingID(worker->getID());
        controller->setGroup(worker->getGroupNum());

        //QStringList lst = worker->getID().split(".");
        //byte idToSend = ((lst.at(0).toInt() << 4) & 0xF0) | (lst.at(1).toInt() & 0x0F);

        sendAnswer(REQ_SERVICE, controller->getSerialNumber(), 21, idToList(worker->getID()));

        ControllerOnLine* cntrl = new ControllerOnLine();
        cntrl->cloneFrom(controller);
        emit(updateControllerBusy(cntrl, false));

        QString workerFIO = worker->getFIO();
        RequestService* request = new RequestService(worker->getID(), date, time);
        request->setCalledControllerID(controller->getID());
        request->setStatus(START_WAIT);
        request->setControllerID(controller->getID());
        request->setWorkerFIO(workerFIO);

        //request->setStatus(HARDWARE_ACCEPT_WAIT);

        addRequest(request);

        //В очередь
    } else if (controllersOnline.count() != 0) {
            RequestService* request = new RequestService(worker->getID(), date, time);
            request->setWorkerFIO(worker->getFIO());
            request->setStatus(ACK_HARDWARE_CALL);
            addRequest(request);
            moveWorkerToQueue(worker, false);
            removeRequest(request, 4);
    } else {
            //нет подкл.пейджеров - неготовность системы
            sendAnswer(ACK_NOREADY, address, 20, (QList<byte>() << 0));
            RequestService* request = new RequestService(worker->getID(), date, time);
            request->setWorkerFIO(worker->getFIO());
            request->setHardwareAcceptTime("Неготовность системы");
            request->setStartTime("Неготовность системы");
            request->setEndTime("Неготовность системы");

            mutexDB->lock();
            DBase->addRequest(date, worker->getID(), time);
            DBase->changeHardwareAnswerReqTime(date, time, "Неготовность системы");
            DBase->changeAcceptReqTime(date, time, "Неготовность системы");
            DBase->changeEndReqTime(date, time, "Неготовность системы", 0);
            mutexDB->unlock();

            qDebug() << "ACK_NOREADY " + addressStr + "(id:" + worker->getID() + ")";
            removeWorkerOnline(worker->getID());
            emit (updateRequest(request, true));
        }

}

void PortWriter::checkReq(WorkerOnLine* worker, RequestService* request, QString date, QString time)
{
    ControllerOnLine* controller = getControllerById(request->getCalledControllerID());
    if (controller != NULL) {
        if (controller->getCallingID() != request->getWorkerID()) {
            controller = getFreeController(worker->getGroupNum());
        }
        if (controller != NULL) {
            QString time = QTime::currentTime().toString("hh:mm:ss");
            controller->setBusy(false);
            controller->setCall(true);
            controller->setCallingID(request->getWorkerID());

            //QStringList lst = request->getWorkerID().split(".");
            //byte idToSend = ((lst.at(0).toInt() << 4) & 0xF0) | (lst.at(1).toInt() & 0x0F);

            sendAnswer(REQ_SERVICE, controller->getSerialNumber(), 20, idToList(request->getWorkerID()));

            request->setControllerID(controller->getID());

            RequestService * req = new RequestService();
            req->cloneFrom(request);
            emit (updateRequest(req, false));
            mutexDB->lock();
            DBase->changeAcceptReqTime(request->getDate(),
                                       request->getRequestTime(),
                                       request->getStartTime());
            mutexDB->unlock();

            ControllerOnLine* cntrl = new ControllerOnLine();
            cntrl->cloneFrom(controller);
            emit(updateControllerBusy(cntrl, false));

        } else if (controllersOnline.count() != 0) {
            WorkerOnLine *w = getWorkerById(request->getWorkerID());
            moveWorkerToQueue(w, false);
            removeRequest(request, 4);

        } else {
            sendAnswer(ACK_NOREADY, worker->getSerialNumber(), 20, (QList<byte>() << 0));
            removeWorkerOnline(request->getWorkerID());
            removeRequest(request, 5);
        }
    } else {
        removeRequest(request, 1);
        WorkerOnLine *w = getWorkerById(request->getWorkerID());
        sendReq(w, date, time);
    }
}

//mode
//0 - удаление запроса, если не было сбоя
//1 - удаление запроса в случае сбоя
//2 - удаление запроса при выключении сервера
//3 - удаление запроса в случае отмены
//4 - сеть занята
//5 - неготовность системы
void PortWriter::removeRequest(RequestService* request, int mode)
{
    if (request != NULL) {
        QString mes;
        mes = "";
        switch (mode) {
            case 0:
            {
                QString endTime = QTime::currentTime().toString("hh:mm:ss");
                request->setEndTime(endTime);
                mutexDB->lock();
                DBase->changeEndReqTime(request->getDate(),
                                        request->getRequestTime(),
                                        request->getEndTime(),
                                        request->getResult());
                mutexDB->unlock();
                break;
            }

            case 1:
                mes = "Сбой";
            case 2:
                if (mes.isNull() || mes.isEmpty()) {
                    mes = "Серв. выкл";
                }
            case 3:
                if (mes.isNull() || mes.isEmpty()) {
                    mes = "Отмена";
                }
            case 4:
                if (mes.isNull() || mes.isEmpty()) {
                    mes = "Сеть занята";
               }
            case 5:
            {
                if (mes.isNull() || mes.isEmpty()) {
                    mes = "Неготовность системы";
                }

                if (request->getStatus() == START_WAIT) {

                    request->setStartTime(mes);
                    request->setEndTime(mes);
                    mutexDB->lock();
                    DBase->changeAcceptReqTime(request->getDate(), request->getRequestTime(), mes);
                    DBase->changeEndReqTime(request->getDate(), request->getRequestTime(), mes, 0);
                    mutexDB->unlock();

                } else if (request->getStatus() == END_WAIT) {

                    request->setEndTime(mes);
                    mutexDB->lock();
                    DBase->changeEndReqTime(request->getDate(),
                                            request->getRequestTime(),
                                            mes,
                                            0);
                    mutexDB->unlock();
                } else {
                    request->setHardwareAcceptTime(mes);
                    request->setStartTime(mes);
                    request->setEndTime(mes);
                    mutexDB->lock();
                    DBase->changeHardwareAnswerReqTime(request->getDate(),
                                                       request->getRequestTime(),
                                                       mes);

                    DBase->changeAcceptReqTime(request->getDate(),
                                               request->getRequestTime(),
                                               mes);

                    DBase->changeEndReqTime(request->getDate(),
                                            request->getRequestTime(),
                                            mes,
                                            0);
                    mutexDB->unlock();
                }
                break;
            }
        }
        requests.remove(requests.indexOf(request));

        RequestService* req = new RequestService();
        req->cloneFrom(request);
        delete request;
        emit (updateRequest(req, false));

    }
}

void PortWriter::checkReq2(WorkerOnLine* worker, RequestService* request, QString date, QString time)
{
    ControllerOnLine* controller = getControllerById(request->getControllerID());
    if (controller != NULL) {
        if (controller->getServiceID() != request->getWorkerID()) {
            controller = getFreeController(worker->getGroupNum());
            if (controller != NULL) {
                removeRequest(request, 1);
                sendReq(worker, date, time);
            } else if (controllersOnline.count() != 0) {
                moveWorkerToQueue(worker, false);
                removeRequest(request, 4);

            } else {
                sendAnswer(ACK_NOREADY, worker->getSerialNumber(), 20, (QList<byte>() << 0));
                removeWorkerOnline(request->getWorkerID());
                removeRequest(request, 5);
            }
        } else {
            sendAnswer(K_REQ_SERVICE, worker->getSerialNumber(), 20, (QList<byte>() << 0));
        }
    } else {
        QString workerID = request->getWorkerID();
        removeRequest(request, 1);
        WorkerOnLine* w = getWorkerById(workerID);
        sendReq(w, date, time);
    }
}

void PortWriter::removeDevAddr(QString address)
{
    if (reqRegAddr.contains(address)) {
        reqRegAddr.removeOne(address);
    }
}

//Генерация и отправка ответа устройству
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

void PortWriter::receiveFrame(char *frame, int size)
{
    QByteArray frm = QByteArray::fromRawData(frame, size);
    qDebug() << "frame:" + XBConvert::addressToString(frm);
    mutexQueue->lock();
    queueOfMessages.enqueue(frm);
    mutexQueue->unlock();

    //delete frame;
}

void PortWriter::updateFIO()
{
    mutexQueue->lock();
    QMap <QString, QString> list1;
    QMap <QString, QString> list2;
    foreach (ControllerOnLine* controller, controllersOnline) {
        QString id = controller->getID();
        QString FIO = DBase->getControllerFIO(id.toInt());
        list1[id] = FIO;
        controller->setFIO(FIO);
    }
    emit updateControllerFIO(list1);

    foreach (WorkerOnLine* worker, workersOnline) {
        QString id = worker->getID();
        QString FIO = DBase->getWorkerFIO(id);
        list2[id] = FIO;
        worker->setFIO(FIO);
    }
    emit updateWorkerFIO(list2);

    mutexQueue->unlock();



}

void PortWriter::terminate1()
{
    this->terminate();
    while (controllersOnline.count() != 0){
        ControllerOnLine* controller = controllersOnline.at(0);
        QString time = QTime::currentTime().toString("hh:mm:ss");
        controller->setTimeOff(time);
        mutexDB->lock();
        DBase->changeControllerTimeOff(controller->getDate(), controller->getTimeOn(), time);
        mutexDB->unlock();
        sendAnswer(POWER_OFF_SEND, controller->getSerialNumber(), 20, (QList<byte>() << 0));
        controllersOnline.remove(0);
        emit(updateControllerBusy(controller, false));
    }

    while (workersOnline.count() != 0) {
        WorkerOnLine* worker = workersOnline.at(0);
        sendAnswer(POWER_OFF_SEND, worker->getSerialNumber(), 20, (QList<byte>() << 0));
        workersOnline.remove(0);
        delete worker;
    }
   /* qDeleteAll(workersOnline);
    workersOnline.clear();*/


    while (requests.count() != 0){
        RequestService* request = requests.at(0);
        removeRequest(request, 2);
    }

    this->m_port->Close();
    //delete m_port;
}

PortWriter::~PortWriter()
{

}
