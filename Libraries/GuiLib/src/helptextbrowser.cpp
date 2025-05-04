#include "helptextbrowser.hpp"
#include <QFileInfo>

namespace GUI {

const char* HelpTextBrowser::VIRTUAL_FOLDER = "doc";

HelpTextBrowser::HelpTextBrowser(QHelpEngine *helpEngine, QWidget *parent) : QTextBrowser (parent),
    m_helpEngine(helpEngine)
{
    auto result = QObject::connect(this,
                                   &HelpTextBrowser::anchorClicked,
                                   this,
                                   &HelpTextBrowser::on_anchorClicked);
    Q_ASSERT(result);
}

QVariant HelpTextBrowser::loadResource(int type, const QUrl &name)
{
    if ( type == QTextDocument::ImageResource ) {
        QFileInfo fileInfo(name.toString());

        return QImage(QString(":/%1").arg(fileInfo.fileName()));
    }

    return QTextBrowser::loadResource(type, name);
}

void HelpTextBrowser::showUrlContent(const QUrl &url, bool rememberUrl)
{
    QString urlStr = url.toString();

    QString anchor;
    QString address;

    splitUrlStr(urlStr, address, anchor);

    if ( m_currentAddress != address ) {
        m_currentAddress = address;
        setHtml(m_helpEngine->fileData(address));
    }

    if ( rememberUrl ) {
         pushUrl(url);
    }

    scrollToAnchor(anchor);
}

void HelpTextBrowser::backward()
{
    if ( m_urlCache.count() ) {
        if ( m_currentUrlIt > m_urlCache.begin() ) {
            m_currentUrlIt--;
            showUrlContent(*m_currentUrlIt);
        }

        checkBackward();
        checkForward();
    }
}

void HelpTextBrowser::forward()
{
    if ( m_urlCache.count() ) {
        if ( m_urlCache.end() - m_currentUrlIt >= 2 ) {
            m_currentUrlIt++;
            showUrlContent(*m_currentUrlIt);
        }

        checkBackward();
        checkForward();
    }
}

void HelpTextBrowser::clearCache()
{
    m_urlCache.clear();
    emit backExists(false);
    emit forwardExists(false);
}

void HelpTextBrowser::on_anchorClicked(const QUrl &url)
{
    QString urlStr = url.toString();
    if ( !urlStr.isEmpty() ) {
        if ( urlStr.at(0) !='#' ) {

            auto docList = m_helpEngine->registeredDocumentations();

            if ( docList.size() ) {
                showUrlContent(QString("qthelp://%1/%2/%3")
                               .arg(docList.at(0))
                               .arg(QString(VIRTUAL_FOLDER))
                               .arg(urlStr), true);
            }
        } else {
            QUrl next = QUrl(m_currentAddress + urlStr);
            pushUrl(next);
        }
    }
}

void HelpTextBrowser::splitUrlStr(const QString &urlStr, QString &address, QString &anchor)
{
    int indA = urlStr.lastIndexOf('#');

    anchor = ( indA != -1 ? urlStr.mid(indA + 1) : QString() );
    address = ( indA != -1 ? urlStr.left(indA) : urlStr );
}

void HelpTextBrowser::pushUrl(const QUrl &url)
{
    if ( m_urlCache.count() == 0 ) {
        m_urlCache.push_back(url);
        m_currentUrlIt = m_urlCache.begin();
        emit backExists(false);
        emit forwardExists(false);
    } else {
        auto tmpIt = m_urlCache.end() - 1;

        while ( tmpIt != m_currentUrlIt ) {
            tmpIt--;
            m_urlCache.removeLast();
        }

        QUrl back = m_urlCache.back();

        if ( back != url ) {
            m_urlCache.push_back(url);
            m_currentUrlIt++;
            emit backExists(true);
            emit forwardExists(false);
        }
    }
}

void HelpTextBrowser::checkForward()
{
    if ( m_urlCache.end() - m_currentUrlIt >= 2 ) {
        emit forwardExists(true);
    } else {
        emit forwardExists(false);
    }
}

void HelpTextBrowser::checkBackward()
{
    if ( m_currentUrlIt > m_urlCache.begin() ) {
        emit backExists(true);
    } else {
        emit backExists(false);
    }
}

} //GUI
