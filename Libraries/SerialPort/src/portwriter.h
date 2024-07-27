#ifndef PORTWRITER_H
#define PORTWRITER_H

#include <QThread>
#include <QMutex>
#include <QDebug>
#include <QMap>
#include <QQueue>

#include "QSerialPort.h"
#include "CSOK_DBase.h"
#include "XBeeTypes.h"
#include "controlleronline.h"
#include "workeronline.h"
#include "requestservice.h"

//#define HARDWARE_ACK_WAIT_SEC 10
#define START_TIME_WAIT_SEC   60
#define END_TIME_WAIT_SEC     1800
//#define REQUEST_PERIOD        20

class PortWriter : public QThread
{
    Q_OBJECT
public:
    explicit PortWriter(QObject *parent = 0, QSerialPort *port = NULL, CSOK_DBase *DBase = NULL, QMutex *mutexDB = NULL);
    void terminate1();
    void run();
    ~PortWriter();

signals:
    void chargeUpdate(QVector <ControllerOnLine*> controllers, QVector <WorkerOnLine*> workers);
    void reqNewDevice(QString serialNumber);
    void updateControllerBusy(ControllerOnLine* controllerOnline, bool flNew);
    void updateControllerFIO(QMap <QString, QString> list);
    void updateControllersBusy(QVector<ControllerOnLine*> controllersOnline);
    void updateRequest(RequestService* request, bool flNew);
    void updateRequests(QVector<RequestService*> requests);
    void updateWorkerFIO(QMap <QString, QString> list);

private:
    QSerialPort *m_port;
    CSOK_DBase *DBase;
    QMutex *mutexDB;
    QMutex *mutexQueue;
    QQueue<QByteArray> queueOfMessages;

    QMap<QByteArray, WorkerOnLine*> workersOnlineMap;
    QMap<QByteArray, ControllerOnLine*> controllersOnlineMap;

    QVector<WorkerOnLine*> workersOnline;         //Рабочие в сети
    QVector<ControllerOnLine*> controllersOnline; //Контроллеры в сети
    QVector<RequestService*> requests;            //Исполняющиеся запросы
    QStringList reqRegAddr;

    QMap<qint32, qint32> groupController; // Какая группа каким контролером обслуживается
    QQueue< QMap <qint8, QQueue <WorkerOnLine*> > > queueByGroups; //Очередь по группам

    void decodeInputFrame(QByteArray inputFrame);
    bool sendAnswer(byte frameID, QByteArray outputAddress, int length, QList <byte> params);
    byte CheckSumm_Calc(byte* data, int length);

    WorkerOnLine* getWorkerOnlineByAddress(QByteArray serialNumber);
    ControllerOnLine* getControllerOnlineByAddress(QByteArray serialNumber);
    ControllerOnLine* getFreeController(int groupOfNewWorker);

    RequestService* getRequestByWorkerId(QString id);
    RequestService *getRequestByControllerId(QString id);

    void addWorker(WorkerOnLine* worker);
    void addController(ControllerOnLine* controller);
    void addRequest(RequestService* request);

    void removeWorkerOnline(WorkerOnLine* worker);
    void removeControllerOnline(ControllerOnLine* controller);

    void removeWorkerOnline(QString id);
    void moveControllerToEnd(ControllerOnLine* controller);

    void sendReq(WorkerOnLine *worker, QString date, QString time);

    void checkReq(WorkerOnLine *worker, RequestService* request, QString date, QString time);
    void checkReq2(WorkerOnLine* worker, RequestService* request, QString date, QString time);

    void removeDevAddr(QString address);

    QString getWorkerOnlineID(QByteArray serialNumber);


    WorkerOnLine* getWorkerById(QString id);


    ControllerOnLine* getControllerById(QString id);

    void moveWorkerToQueue(WorkerOnLine *worker, bool first);
    void unQueueWorker(ControllerOnLine* freeController);

    void removeRequest(RequestService* request, int mode);
    WorkerOnLine* getWorkerByGroup(int group);

    void dispatchOnline();
    void removeAllWorkers();

    QList<byte> idToList(QString id);
    QString getId(quint8 group, quint8 id);

public slots:
    void receiveFrame(char* frame, int size);
    void updateFIO();
    
};

#endif // PORTWRITER_H
