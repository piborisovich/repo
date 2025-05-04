#pragma once

#include "gui_global.hpp"

#include <QHelpEngine>
#include <QTextBrowser>

namespace GUI {

/*!
 * \brief Расширение QTextBrowser для работы с HelpEngine с возможностью переназначать пути к изображениям
 */
#ifdef LIB_EXPORT
class LIB_EXPORT HelpTextBrowser : public QTextBrowser
#else
class HelpTextBrowser : public QTextBrowser
#endif
{
    Q_OBJECT
public:
    HelpTextBrowser(QHelpEngine *helpEngine, QWidget* parent = nullptr);

    QVariant loadResource(int type, const QUrl &name) override;

    void showUrlContent(const QUrl &url, bool rememberUrl = false);

public slots:
    void backward() override;
    void forward() override;

    void clearCache();

signals:
    void backExists(bool exists);
    void forwardExists(bool exists);

private slots:
    void on_anchorClicked(const QUrl &);

private:
    static void splitUrlStr(const QString &urlStr, QString &address, QString &anchor);

    void pushUrl(const QUrl &url);
    void checkForward();
    void checkBackward();

private:
    static const char* VIRTUAL_FOLDER;

    QHelpEngine *m_helpEngine;
    QString m_currentAddress;

    QList<QUrl> m_urlCache;
    QList<QUrl>::iterator m_currentUrlIt;
};

} //GUI
