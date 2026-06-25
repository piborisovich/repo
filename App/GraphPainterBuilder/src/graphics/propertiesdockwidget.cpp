#include "propertiesdockwidget.hpp"
#include "linepropertieswidget.hpp"
#include "strings.hpp"

#include "lineitem.hpp"
#include "pathitem.hpp"

#include <QVBoxLayout>
#include <QColorDialog>

PropertiesDockWidget::PropertiesDockWidget(const QString &title,
                                           QGraphicsItem *graphicsItem,
                                           QWidget *parent,
                                           Qt::WindowFlags flags)
    : QDockWidget(title, parent, flags)
    , m_colorButton( new QPushButton() )
    , m_propertiesWidget(nullptr)
{
    setFeatures(QDockWidget::DockWidgetMovable |
                QDockWidget::DockWidgetFloatable);

    QWidget *contentWidget = new QWidget(this);

    QVBoxLayout* widgetLayout = new QVBoxLayout(contentWidget);

    widgetLayout->setContentsMargins(5, 5, 5, 5);
    widgetLayout->addWidget(m_colorButton);
    widgetLayout->addSpacing(10);

    m_propertiesWidget = createContent(graphicsItem);

    if ( m_propertiesWidget ) {
        setColor(m_propertiesWidget->color());
        widgetLayout->addWidget( m_propertiesWidget );
    }

    widgetLayout->addStretch();

    setWidget( contentWidget );

    auto result = connect(m_colorButton,
                          &QPushButton::clicked,
                          this,
                          &PropertiesDockWidget::on_colorClicked);
    Q_ASSERT(result);
}

void PropertiesDockWidget::setColor(const QColor &color)
{
    m_colorButton->setStyleSheet(QString("background-color: %1;"
                                         "min-height: 30px;"
                                         "border: 1px solid #555;").arg(color.name()));
}

void PropertiesDockWidget::on_colorClicked()
{
    QColor color = QColorDialog::getColor(m_colorButton->palette().button().color(),
                                          this,
                                          Strings::SELECT_COLOR_TEXT);
    if (color.isValid()) {
        setColor(color);
        if ( m_propertiesWidget ) {
            m_propertiesWidget->setColor(color);
        }
    }
}

ItemPropertiesWidget *PropertiesDockWidget::createContent(QGraphicsItem *item)
{
    LineItem* lineItem =  dynamic_cast<LineItem*>(item);

    if ( lineItem ) {
        return new LinePropertiesWidget(lineItem);
    }

    PathItem* pathItem =  dynamic_cast<PathItem*>(item);

    return nullptr;
}
