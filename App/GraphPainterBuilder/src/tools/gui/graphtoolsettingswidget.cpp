#include "graphtoolsettingswidget.hpp"
#include "graphtoolsettings.hpp"

#include <QLayout>

GraphToolSettingsWidget::GraphToolSettingsWidget(std::shared_ptr<ToolSettings> settings,
                                                 QWidget* parent,
                                                 Qt::WindowFlags f)
    : ToolSettingsWidget(settings, parent, f)
    , m_pointsCnt(new QSpinBox())
    , m_plotButton( new QPushButton("Plot"))
{
    m_pointsCnt->setMinimum(2);
    m_pointsCnt->setMaximum(std::numeric_limits<int>::max());

    auto graphSettings = dynamic_cast<GraphToolSettings*>(settings.get());

    if ( graphSettings )  {
        m_pointsCnt->setValue(graphSettings->pointsCount());
    }


    addWidget("<b>Points:</b>", m_pointsCnt);
    addWidget(m_plotButton);

    auto result = connect(m_pointsCnt,
                          &QSpinBox::valueChanged,
                          this,
                          &GraphToolSettingsWidget::on_pointsCntChanged);
    Q_ASSERT(result);

    result = connect(m_plotButton,
                     &QPushButton::clicked,
                     this,
                     &GraphToolSettingsWidget::startPlot);
    Q_ASSERT(result);
}

void GraphToolSettingsWidget::on_pointsCntChanged(int value)
{
    auto graphSettings = dynamic_cast<GraphToolSettings*>(m_settings.get());

    if ( graphSettings )  {
        graphSettings->setPointsCount(value);
    }
}
