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
    sizeSlider->setValue( settings->brushSize() * 10 );

    m_layout->addRow(sizeSlider);
    m_layout->addRow(Strings::BRUSH_SIZE_TEXT, m_sizeLabel);

    auto result = connect(sizeSlider,
                          &QSlider::valueChanged,
                          this,
                          &ToolSettingsWidget::on_brushSizeChanged);
    Q_ASSERT(result);

    on_brushSizeChanged( sizeSlider->value() );
}

void ToolSettingsWidget::addWidget(QWidget *widget)
{
    m_layout->addWidget(widget);
}

void ToolSettingsWidget::addWidget(const QString &name, QWidget *widget)
{
    m_layout->addRow(name, widget);
}

void ToolSettingsWidget::on_brushSizeChanged(int size)
{
    size /= 10.0;
    m_sizeLabel->setText( QString::number(size) );
    if ( m_settings ) {
        m_settings->setBrushSize(size);
    }
}
