#ifndef CORE_HPP
#define CORE_HPP

#include "Utils.h"

#include <QCoreApplication>
#include <QDateTime>

#include <vector>
#include <memory>

#define logDebug() qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz")

/*!
 * \brief Ядро
 */
class Core : public QCoreApplication
{
    Q_OBJECT

    class State;
    class WaitToSelectConverterState;
    class FindControllersState;
public:

    Core(int &argc, char **argv);
    ~Core();

    void changeState(State * state);

protected:
    void timerEvent(QTimerEvent *) override;

private:
    void init();
    void doEnumConverters(); //!< Подготовить список конвертеров

private:

    CZGuardLoader m_guardLoader;

    std::vector< std::shared_ptr<_ZG_CVT_OPEN_PARAMS> > m_converters; //!< Информация о найденных конвертерах

    State *m_state;

};

class Core::State
{
public:
    static State *instance();

    virtual void exec(Core *core, const std::vector< std::shared_ptr<_ZG_CVT_OPEN_PARAMS> > &converters);

    friend WaitToSelectConverterState;
    friend FindControllersState;

protected:
    void changeState(Core *core, State* newState);

private:
    State();
};

class Core::WaitToSelectConverterState : public Core::State
{
public:
    static WaitToSelectConverterState *instance();

    virtual void exec(Core *core, const std::vector< std::shared_ptr<_ZG_CVT_OPEN_PARAMS> > &converters) override;

private:
    WaitToSelectConverterState();
};

class Core::FindControllersState : public Core::State
{
public:
    static FindControllersState *instance(int id);

    virtual void exec(Core *core, const std::vector< std::shared_ptr<_ZG_CVT_OPEN_PARAMS> > &converters) override;

    int converterID() const;
    void setConverterID(int newConverterID);

private:
    FindControllersState();


private:
    int m_converterID;
    HANDLE g_hCvt;
};

#endif // CORE_HPP
