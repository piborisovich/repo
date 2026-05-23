#ifndef CORE_HPP
#define CORE_HPP

#define logDebug() qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz")

#include <QApplication>

class Core : public QApplication
{
    Q_OBJECT

public:
    Core(int &argc, char **argv);

    ~Core();

public slots:
    void on_ImageOpen();

private:
    Q_DISABLE_COPY(Core)
};
#endif // CORE_HPP
