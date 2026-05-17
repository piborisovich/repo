#include "core.hpp"

#include <QCommandLineParser>
#include <QMap>
#include <iostream>

static const char *APPLICATION_NAME = "IronLogicGuardTest";

static const ZG_CVT_SPEED PORT_SPEED = ZG_SPEED_57600;
static const ZP_PORT_TYPE PORT_TYPE = ZP_PORT_COM;

using HEADER_PAIR=std::pair<QString, int>;

Q_GLOBAL_STATIC( HEADER_PAIR, DEVCOUNT_HEADER,    { "Device"    , 10 } );
Q_GLOBAL_STATIC( HEADER_PAIR, DEVTYPE_HEADER,     { "Type"      , 20 } );
Q_GLOBAL_STATIC( HEADER_PAIR, DEVMODE_HEADER,     { "Mode"      , 10 } );
Q_GLOBAL_STATIC( HEADER_PAIR, DEVSN_HEADER,       { "S/n"       , 10 } );
Q_GLOBAL_STATIC( HEADER_PAIR, DEVVERSION_HEADER,  { "Version"   , 10 } );
Q_GLOBAL_STATIC( HEADER_PAIR, DEVPORT_HEADER,     { "Port"      , 8  } );
Q_GLOBAL_STATIC( HEADER_PAIR, DEVPORTTYPE_HEADER, { "Port type" , 10 } );

Q_GLOBAL_STATIC(const QString, tableLineTemplate, "| %1| %2| %3| %4| %5| %6| %7|");

inline QString converterVersionString(UINT ver)
{
     return QString("%1.%2.%3").arg(ver & 0xff)
                               .arg((ver >> 8) & 0xff)
                               .arg((ver >> 16) & 0xff);
}

Core::Core(int &argc, char **argv)
    : QCoreApplication(argc, argv),
    m_state(nullptr)
{
    m_converters.reserve(10);

    init();
    doEnumConverters();

    startTimer(100);
}

Core::~Core()
{
}

void Core::changeState(State *state)
{
    m_state = state;
}

void Core::timerEvent(QTimerEvent *)
{
    if ( m_guardLoader.IsLoaded() ) {
        if ( m_converters.size() && m_state ) {
            m_state->exec( this, m_converters );
        } else {
            qCritical() << "Converters not found";
            quit();
        }
    } else {
        qCritical() << "Loading ZGuard error";
        quit();
    }
}

void Core::init()
{
    QCommandLineParser parser;

    setApplicationName(APPLICATION_NAME);
    setApplicationVersion( m_guardLoader.IsLoaded() ? QString("%1.%2.%3")
                                                         .arg( ZG_GetVersion() & 0xFF  )
                                                         .arg( (ZG_GetVersion() >> 8) & 0xFF  )
                                                         .arg( (ZG_GetVersion() >> 16) & 0xFF  )
                                                    : "");

    parser.setApplicationDescription("IronLogicGuardTest helper");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(*this);
}

void Core::doEnumConverters()
{
    if ( m_guardLoader.IsLoaded() ) {

        HANDLE hSearch;
        _ZP_SEARCH_PARAMS rSP;
        ZeroMemory(&rSP, sizeof(rSP));


        QString tableHeader = tableLineTemplate->arg(DEVCOUNT_HEADER->first, -DEVCOUNT_HEADER->second)
                                                .arg(DEVTYPE_HEADER->first, -DEVTYPE_HEADER->second)
                                                .arg(DEVMODE_HEADER->first, -DEVMODE_HEADER->second)
                                                .arg(DEVSN_HEADER->first, -DEVSN_HEADER->second)
                                                .arg(DEVVERSION_HEADER->first, -DEVVERSION_HEADER->second)
                                                .arg(DEVPORT_HEADER->first, -DEVPORT_HEADER->second)
                                                .arg(DEVPORTTYPE_HEADER->first, -DEVPORTTYPE_HEADER->second);
        QString line(tableHeader.size() - 3, '-');

        qDebug() << "Scan for devices...";
        qDebug().noquote() <<" " << line;
        qDebug().noquote() << tableHeader;
        qDebug().noquote() <<" " << line;

        if ( ZG_SearchDevices(&hSearch, &rSP, TRUE, FALSE) == S_OK ) {
            _ZG_ENUM_IPCVT_INFO rInfo;
            _ZP_PORT_INFO aPIs[2];
            PZP_PORT_INFO pPI;
            INT nPortCount;
            INT_PTR nDevCount = 0;

            while ( ZG_FindNextDevice(hSearch, &rInfo, aPIs, _countof(aPIs), &nPortCount, INFINITE) == S_OK )
            {
                PZG_CVT_OPEN_PARAMS pOp = new _ZG_CVT_OPEN_PARAMS();

                ZeroMemory(pOp, sizeof(*pOp));

                ++nDevCount;
                pPI = &aPIs[0];

                auto w_len = wcslen(pPI->szFriendly);
                wchar_t* pPortNameBuffer = new wchar_t[w_len + 1];

                pOp->nType = PORT_TYPE;
                pOp->nCvtType = rInfo.nType;
                pOp->pszName = pPortNameBuffer;
                wcscpy_s(pPortNameBuffer, w_len + 1, pPI->szFriendly);
                pOp->nSpeed = PORT_SPEED;
                pOp->nStopBits = ONESTOPBIT;

                m_converters.push_back(std::shared_ptr<_ZG_CVT_OPEN_PARAMS>(pOp));

                qDebug().noquote() << tableLineTemplate->arg(nDevCount, -DEVCOUNT_HEADER->second)
                                                       .arg(CvtTypeStrs[rInfo.nType], -DEVTYPE_HEADER->second)
                                                       .arg(GuardModeStrs[rInfo.nMode], -DEVMODE_HEADER->second)
                                                       .arg(rInfo.nSn, -DEVSN_HEADER->second)
                                                       .arg( converterVersionString(rInfo.nVersion), -DEVVERSION_HEADER->second  )
                                                       .arg(pPI->szFriendly, -DEVPORT_HEADER->second)
                                                       .arg(PortTypeStrs[pPI->nType], -DEVPORTTYPE_HEADER->second );

            }

            ZG_CloseHandle(hSearch);

            if ( nDevCount ) {
                changeState( WaitToSelectConverterState::instance() );
                qDebug().noquote() <<" " << line;
            }
        }

    }
}

Core::State *Core::State::instance()
{
    static State* state = new State();

    return state;
}

void Core::State::exec(Core *core, const std::vector<std::shared_ptr<_ZG_CVT_OPEN_PARAMS> > &converters)
{
    Q_UNUSED(core);
    Q_UNUSED(converters);
}

void Core::State::changeState(Core *core, State *newState)
{
    core->changeState(newState);
}

Core::State::State()
{
}

Core::WaitToSelectConverterState *Core::WaitToSelectConverterState::instance()
{
    static WaitToSelectConverterState* state = new WaitToSelectConverterState();

    return state;
}

void Core::WaitToSelectConverterState::exec(Core *core, const std::vector<std::shared_ptr<_ZG_CVT_OPEN_PARAMS> > &converters)
{
    Q_UNUSED(converters);

    QTextStream in(stdin);
    QTextStream out(stdout);

    int convereter_index = -1;

    out << "Select converter ";
    out.flush();
    in >> convereter_index;

    core->changeState( FindControllersState::instance(convereter_index) );
}

Core::WaitToSelectConverterState::WaitToSelectConverterState()
{
}

Core::FindControllersState *Core::FindControllersState::instance(int id)
{
    static FindControllersState* state = new FindControllersState();

    state->setConverterID(id);

    return state;
}

void Core::FindControllersState::exec(Core *core, const std::vector<std::shared_ptr<_ZG_CVT_OPEN_PARAMS> > &converters)
{
    try {
        auto &converter = converters.at(m_converterID - 1);
        HRESULT hr;

        if ( ( hr = ZG_Cvt_Open(&g_hCvt, converter.get(), NULL) ) == S_OK ) {
            std::cout << "Search controllers...\n";
            std::string str;
            std::getline(std::cin, str);
            std::getline(std::cin, str);
        } else {
            std::cout << "Port not opened. Error " << std::hex << hr << std::endl;

            core->changeState( WaitToSelectConverterState::instance() );
        }

    } catch(std::out_of_range &ex) {
        qCritical() << ex.what();
        core->quit();
    }

}

Core::FindControllersState::FindControllersState() :
    m_converterID(-1),
    g_hCvt(NULL)
{
}

int Core::FindControllersState::converterID() const
{
    return m_converterID;
}

void Core::FindControllersState::setConverterID(int newConverterID)
{
    m_converterID = newConverterID;
}
