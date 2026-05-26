#ifndef CORE_HPP
#define CORE_HPP

#define logDebug() qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz")

#include "toollist.hpp"

#include <QApplication>
#include <QUndoStack>

class Core : public QApplication
{
    Q_OBJECT

public:
    Core(int &argc, char **argv);

    ~Core();

    static const ToolList &tools();

private:
    Q_DISABLE_COPY(Core)

    static ToolList _tools;
};
#endif // CORE_HPP
