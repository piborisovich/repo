#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <SimplePortListener>

#include <QDateTime>

#define logDebug() qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz")

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum DATA_TYPES
    {
        HEX_DATA,
        ASCII_DATA
    };

signals:
    void disconnectPort();
    void sendData(QByteArray data);

private:
    void init();
    QMap<quint8, QString> &getDataTypes() const
    {
        static QMap<quint8, QString> dataTypes = {
            std::make_pair(static_cast<quint8>(HEX_DATA), QString("HEX")),
            std::make_pair(static_cast<quint8>(ASCII_DATA), QString("ASCII"))
        };

        return dataTypes;
    }

private slots:
    void onPortConnect();
    void onPortDisconnect();
    void onPortSend();
    void onPortConnected( bool connected);
    void onPortDataReceived(QByteArray data);

private:
    static const int MAX_PORT_NUMBERS;
    Ui::MainWindow *ui;
    QScopedPointer<SimplePortListener> m_simplePortListener;
};
#endif // MAINWINDOW_HPP
