#include "helpwidget.hpp"
#include "ui_helpwidget.h"

#include <QHelpContentWidget>
#include <QHelpIndexWidget>
#include <QHelpSearchEngine>
#include <QHelpSearchQueryWidget>
#include <QHelpSearchResultWidget>

#include <QPushButton>
#include <QLabel>
#include <QToolButton>

#include <QTimer>
#include <QDebug>

namespace GUI {

QMap<QString, int> HelpWidget::helpModes = {
                                             std::make_pair( QString("Содержание"), int(HELP_MODES::CONTENT) ),
                                             std::make_pair( QString("Индекс"),     int(HELP_MODES::INDEXES) ),
                                             std::make_pair( QString("Поиск"),      int(HELP_MODES::SEARCH)  ),
                                           };

const char *HelpWidget::BACK_TOOLTIP = "Назад";
const char *HelpWidget::FORWARD_TOOLTIP = "Вперед";

HelpWidget::HelpWidget(const QString &helpFilePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpWidget),
    m_helpEngine( new QHelpEngine( helpFilePath, this) ),
    m_textBrowser( new HelpTextBrowser(m_helpEngine, this))
{
    ui->setupUi(this);

    ui->vLayoutTextBrowser->addWidget(m_textBrowser);
    ui->vLayoutTextBrowser->setStretch(0, 0);
    ui->vLayoutTextBrowser->setStretch(1, 1);
    ui->backButton->setToolTip(QString(BACK_TOOLTIP));
    ui->forwardButton->setToolTip(QString(FORWARD_TOOLTIP));

    setWindowFlag(Qt::Window, true);
    setAttribute(Qt::WA_DeleteOnClose, true);

    init();
}

HelpWidget::~HelpWidget()
{
    delete ui;
}

void HelpWidget::on_selectIndex(const QModelIndex &index)
{
    if ( index.isValid() ) {
        QHelpContentItem *item = m_helpEngine->contentModel()->contentItemAt(index);
        m_textBrowser->clearCache();
        m_textBrowser->showUrlContent(item->url(), true);
    }
}

void HelpWidget::on_setupStarted()
{
}

void HelpWidget::on_setupFinished()
{
    QHelpSearchQueryWidget *helpSearchWidget = m_helpEngine->searchEngine()->queryWidget();

    QList<QPushButton*> buttons = helpSearchWidget->findChildren<QPushButton*>("");

    if ( buttons.count() ) {
        for (QPushButton * button : buttons ) {
            if ( button->text() == "Search" ) {
                button->setText("Поиск");
            }
        }
    }

    trResultWidget();
}

void HelpWidget::on_warning(const QString &msg)
{
    qWarning() << msg;
}

void HelpWidget::on_contentsCreated()
{
    QModelIndex firstIndex = m_helpEngine->contentModel()->index(0, 0, m_helpEngine->contentWidget()->rootIndex());
    on_selectIndex( firstIndex );

    m_helpEngine->contentWidget()->expandAll();
    m_helpEngine->contentWidget()->selectionModel()->select(firstIndex, QItemSelectionModel::Select);
}

void HelpWidget::on_searchRequest()
{
    QString searchInput = m_helpEngine->searchEngine()->queryWidget()->searchInput();
    m_helpEngine->searchEngine()->search(searchInput);
}

void HelpWidget::on_requestShowLink(const QUrl &url)
{
    m_textBrowser->clearCache();
    m_textBrowser->showUrlContent(url, true);
}

void HelpWidget::on_linkActivated(const QUrl &link, const QString &keyword)
{
    Q_UNUSED(keyword);
    m_textBrowser->clearCache();
    m_textBrowser->showUrlContent(link, true);
}

void HelpWidget::on_indexingStarted()
{
}

void HelpWidget::on_indexingFinished()
{
}

void HelpWidget::on_searchingStarted()
{
    m_helpEngine->searchEngine()->queryWidget()->setEnabled(false);
}

void HelpWidget::on_searchingFinished(int searchResultCount)
{
    Q_UNUSED(searchResultCount);

    m_helpEngine->searchEngine()->queryWidget()->setEnabled(true);

    QTimer::singleShot(10, this, &HelpWidget::trResultWidget);
}

void HelpWidget::on_selectorIndexChanged(int index)
{
    if ( index >= 0 ) {
        int mode = ui->selectorBox->itemData(index).toInt();
        switch (mode) {
        case int(HELP_MODES::SEARCH):
        case int(HELP_MODES::CONTENT):
        case int(HELP_MODES::INDEXES):
            setView( static_cast<HELP_MODES>(mode) );
            break;
        };
    }
}

void HelpWidget::init()
{
    ui->splitter->setContentsMargins(5, 5, 5, 5);

    m_textBrowser->setOpenLinks(true);
    m_textBrowser->setOpenExternalLinks(true);

    ui->selectorBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    auto result = QObject::connect(m_helpEngine,
                                   &QHelpEngine::setupStarted,
                                   this,
                                   &HelpWidget::on_setupStarted);
    Q_ASSERT(result);

    result = QObject::connect(m_helpEngine,
                              &QHelpEngine::setupFinished,
                              this,
                              &HelpWidget::on_setupFinished);
    Q_ASSERT(result);

    result = QObject::connect(m_helpEngine,
                              &QHelpEngine::warning,
                              this,
                              &HelpWidget::on_warning);
    Q_ASSERT(result);

    result = QObject::connect(m_helpEngine->contentModel(),
                              &QHelpContentModel::contentsCreated,
                              this,
                              &HelpWidget::on_contentsCreated);
    Q_ASSERT(result);


    result = QObject::connect(m_helpEngine->contentWidget(),
                              &QHelpContentWidget::clicked,
                              this,
                              &HelpWidget::on_selectIndex);
    Q_ASSERT(result);

    result = QObject::connect(ui->selectorBox,
                              QOverload<int>::of(&QComboBox::currentIndexChanged),
                              this,
                              &HelpWidget::on_selectorIndexChanged);
    Q_ASSERT(result);

    result = QObject::connect(m_helpEngine->searchEngine(),
                              &QHelpSearchEngine::indexingStarted,
                              this,
                              &HelpWidget::on_indexingStarted);
    Q_ASSERT(result);

    result = QObject::connect(m_helpEngine->searchEngine(),
                              &QHelpSearchEngine::indexingFinished,
                              this,
                              &HelpWidget::on_indexingFinished);
    Q_ASSERT(result);

    result = QObject::connect(m_helpEngine->searchEngine(),
                              &QHelpSearchEngine::searchingStarted,
                              this,
                              &HelpWidget::on_searchingStarted);
    Q_ASSERT(result);

    result = QObject::connect(m_helpEngine->searchEngine(),
                              &QHelpSearchEngine::searchingFinished,
                              this,
                              &HelpWidget::on_searchingFinished);
    Q_ASSERT(result);

    result = QObject::connect(m_helpEngine->searchEngine()->queryWidget(),
                              &QHelpSearchQueryWidget::search,
                              this,
                              &HelpWidget::on_searchRequest);
    Q_ASSERT(result);

    result = QObject::connect(m_helpEngine->searchEngine()->resultWidget(),
                              &QHelpSearchResultWidget::requestShowLink,
                              this,
                              &HelpWidget::on_requestShowLink);
    Q_ASSERT(result);

    result = QObject::connect(m_helpEngine->indexWidget(),
                              &QHelpIndexWidget::linkActivated,
                              this,
                              &HelpWidget::on_linkActivated);
    Q_ASSERT(result);

    result = QObject::connect(m_textBrowser,
                              &HelpTextBrowser::backExists,
                              ui->backButton,
                              &QToolButton::setEnabled);
    Q_ASSERT(result);

    result = QObject::connect(m_textBrowser,
                              &HelpTextBrowser::forwardExists,
                              ui->forwardButton,
                              &QToolButton::setEnabled);
    Q_ASSERT(result);

    result = QObject::connect(ui->backButton,
                              &QToolButton::clicked,
                              m_textBrowser,
                              &HelpTextBrowser::backward);
    Q_ASSERT(result);

    result = QObject::connect(ui->forwardButton,
                              &QToolButton::clicked,
                              m_textBrowser,
                              &HelpTextBrowser::forward);
    Q_ASSERT(result);

    fillCombo();
    setCurrentMode(HELP_MODES::CONTENT);

    ui->backButton->setEnabled(false);
    ui->forwardButton->setEnabled(false);

    m_helpEngine->setupData();
}

void HelpWidget::fillCombo()
{
    for ( auto it = helpModes.begin(); it != helpModes.end(); it++ ) {
        ui->selectorBox->addItem(it.key(), it.value());
    }
}

void HelpWidget::setCurrentMode(HelpWidget::HELP_MODES mode)
{
    for ( int i = 0; i < ui->selectorBox->count(); ++i ) {
        if ( ui->selectorBox->itemData(i).toInt() == int(mode) ) {
            ui->selectorBox->setCurrentIndex(i);
            break;
        }
    }
}

void HelpWidget::setView(HelpWidget::HELP_MODES mode)
{
    QVBoxLayout *modeLayout = static_cast<QVBoxLayout*>( ui->modeWidget->layout() );

    if ( modeLayout && modeLayout->count() >= 1 ) {
        while ( modeLayout->count() > 1 ) {
            QObjectList list = modeLayout->children();
            modeLayout->removeItem(modeLayout->itemAt(modeLayout->count() - 1));
        }

        m_helpEngine->contentWidget()->hide();
        m_helpEngine->indexWidget()->hide();
        m_helpEngine->searchEngine()->queryWidget()->hide();
        m_helpEngine->searchEngine()->resultWidget()->hide();

        switch (mode) {
        case HELP_MODES::SEARCH:
            modeLayout->addWidget(m_helpEngine->searchEngine()->queryWidget());
            modeLayout->addWidget(m_helpEngine->searchEngine()->resultWidget());
            m_helpEngine->searchEngine()->queryWidget()->setVisible(true);
            m_helpEngine->searchEngine()->resultWidget()->setVisible(true);
            break;

        case HELP_MODES::CONTENT:
            modeLayout->addWidget(m_helpEngine->contentWidget());
            m_helpEngine->contentWidget()->setVisible(true);
            on_selectIndex(m_helpEngine->contentWidget()->currentIndex());
            break;

        case HELP_MODES::INDEXES:
            modeLayout->addWidget(m_helpEngine->indexWidget());
            m_helpEngine->indexWidget()->setVisible(true);
            break;
        }

        modeLayout->setStretch(modeLayout->count() - 1, 1);
        ui->splitter->setStretchFactor(0, 0);
        ui->splitter->setStretchFactor(1, 1);
    }
}

void HelpWidget::trResultWidget()
{
    QHelpSearchResultWidget *helpSearchResultWidget = m_helpEngine->searchEngine()->resultWidget();

    QList<QLabel*> labels = helpSearchResultWidget->findChildren<QLabel*>("");

    if ( labels.count() ) {
        for ( QLabel * label : labels ) {
            QString lblText = label->text();

            label->setText( lblText.replace("of", "из").replace("Hits", "совпадений") );
        }
    }
}

} //GUI
