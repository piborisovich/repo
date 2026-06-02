#include "toolswidget.hpp"

#include "toolwidget.hpp"


#include "strings.hpp"

#include <QLabel>
#include <QVBoxLayout>
#include <QColorDialog>
#include <QComboBox>

ToolsWidget::ToolsWidget(const QString &title,
                         const TColorGetter &colorGetter,
                         const ToolList &toolList,
                         QWidget *parent,
                         Qt::WindowFlags flags) : QDockWidget(title, parent, flags)
    , m_colorButton(new QPushButton())
    , m_settingsWidgetLayout(new QVBoxLayout())
    , m_colorGetter(colorGetter)
{
    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout* widgetLayout = new QVBoxLayout(contentWidget);
    ToolWidget* toolWidget = new ToolWidget(toolList, this);

    widgetLayout->setContentsMargins(5, 0, 0, 0);

    widgetLayout->addWidget( toolWidget );

    // Выбор цвета
    widgetLayout->addWidget(new QLabel(Strings::PALETTE_TEXT));
    updateColorButtonLayout(Qt::black);
    widgetLayout->addWidget(m_colorButton);

    widgetLayout->addSpacing(10);
    widgetLayout->addLayout(m_settingsWidgetLayout);

    widgetLayout->addStretch();

    setWidget(contentWidget);

    auto result = connect(toolWidget,
                          &ToolWidget::toolSelected,
                          this,
                          &ToolsWidget::currentToolChanged);
    Q_ASSERT(result);

    result = connect(toolWidget,
                     &ToolWidget::toolSelected,
                     this,
                     &ToolsWidget::on_currentToolChanged);
    Q_ASSERT(result);

    result = connect(toolWidget,
                     &ToolWidget::toolDisabled,
                     this,
                     &ToolsWidget::on_toolDisabled);
    Q_ASSERT(result);

    result = connect(m_colorButton,
                     &QPushButton::clicked,
                     this,
                     &ToolsWidget::on_colorClicked);
    Q_ASSERT(result);
}

void ToolsWidget::on_colorClicked()
{
    try {

        QColor color = QColorDialog::getColor(m_colorGetter(),
                                              this,
                                              Strings::SELECT_COLOR_TEXT);
        if (color.isValid()) {
            emit colorChanged(color);
            updateColorButtonLayout(color);
        }
    } catch (std::bad_function_call &ex) {
        qDebug() << ex.what();
    }
}

void ToolsWidget::on_currentToolChanged(ITool &tool)
{
    on_toolDisabled();
    m_settingsWidgetLayout->addWidget(tool.settingsWidget());
}

void ToolsWidget::on_toolDisabled()
{
    QLayoutItem* layoutItem;
    while ( ( layoutItem = m_settingsWidgetLayout->takeAt(0) ) != nullptr )
    {
        layoutItem->widget()->setParent(nullptr);
        delete layoutItem;
    }
}

void ToolsWidget::updateColorButtonLayout(QColor color)
{
    m_colorButton->setStyleSheet(QString("background-color: %1;"
                                         "min-height: 30px;"
                                         "border: 1px solid #555;").arg(color.name()));
}
