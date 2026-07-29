#include "toolsettingswidget.hpp"
#include "strings.hpp"

#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QFormLayout>

ToolSettingsWidget::ToolSettingsWidget(std::shared_ptr<ToolSettings> settings,
                                       QWidget *parent,
                                       Qt::WindowFlags f) : QWidget(parent, f)
    , m_settings(settings)
    , m_layout(new QFormLayout(this))
    , m_sizeLabel(new QLabel())
{
    QSlider *sizeSlider = new QSlider(Qt::Horizontal);

    sizeSlider->setRange(1, 1000);
    sizeSlider->setValue( settings->penSize() * 10.0 );

    m_layout->addRow(sizeSlider);
    m_layout->addRow(Strings::PEN_SIZE_TEXT, m_sizeLabel);

    auto result = connect(sizeSlider,
                          &QSlider::valueChanged,
                          this,
                          &ToolSettingsWidget::on_penSizeChanged);
    Q_ASSERT(result);

    on_penSizeChanged( sizeSlider->value() );
}

void ToolSettingsWidget::addWidget(QWidget *widget)
{
    m_layout->addWidget(widget);
}

void ToolSettingsWidget::addWidget(const QString &name, QWidget *widget)
{
    m_layout->addRow(name, widget);
}

void ToolSettingsWidget::on_penSizeChanged(int size)
{
    qreal value = size / 10.0;
    m_sizeLabel->setText( QString::number(value, 'f', 1) );
    if ( m_settings ) {
        m_settings->setPenSize(value);
    }
}
