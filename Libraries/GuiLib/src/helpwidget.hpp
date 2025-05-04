#pragma once

#include "gui_global.hpp"

#include "helptextbrowser.hpp"

#include <QWidget>
#include <QTextBrowser>

namespace Ui {
class HelpWidget;
}

namespace GUI {

/*!
 * \brief Окно справки
 */
#ifdef LIB_EXPORT
class LIB_EXPORT HelpWidget : public QWidget
#else
class HelpWidget : public QWidget
#endif
{
    Q_OBJECT

    enum class HELP_MODES
    {
        CONTENT,
        INDEXES,
        SEARCH
    };

public:
    explicit HelpWidget(const QString &helpFilePath, QWidget *parent = nullptr);
    ~HelpWidget();

private slots:
    void on_selectIndex( const QModelIndex &index );
    void on_setupStarted();
    void on_setupFinished();
    void on_warning(const QString &msg);
    void on_contentsCreated();

    void on_searchRequest();

    void on_requestShowLink(const QUrl &url);

    void on_linkActivated(const QUrl &link, const QString &keyword);

    void on_indexingStarted();
    void on_indexingFinished();
    void on_searchingStarted();
    void on_searchingFinished(int searchResultCount);

    void on_selectorIndexChanged(int index);

private:

    void init();
    void fillCombo();
    void setCurrentMode(HELP_MODES mode);
    void setView(HELP_MODES mode);
    void trResultWidget();

private:
    static const char *BACK_TOOLTIP;
    static const char *FORWARD_TOOLTIP;

    static QMap<QString, int> helpModes;

    Ui::HelpWidget *ui;
    QHelpEngine *m_helpEngine;
    HelpTextBrowser *m_textBrowser;
};

} //GUI
