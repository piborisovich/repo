#include "toolsettingswidget.hpp"
#include "strings.hpp"

#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>

ToolSettingsWidget::ToolSettingsWidget(std::shared_ptr<ToolSettings> settings,
                                       QWidget *parent,
                                       Qt::WindowFlags f) : QWidget(parent, f)
    , m_settings(settings)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);

    vLayout->addWidget(new QLabel(Strings::BRUSH_SIZE_TEXT));
    QSlider *sizeSlider = new QSlider(Qt::Horizontal);
    sizeSlider->setRange(1, 100);
    sizeSlider->setValue( settings->brushSize() );
    vLayout->addWidget(sizeSlider);

    auto result = connect(sizeSlider,
                          &QSlider::valueChanged,
                          this,
                          &ToolSettingsWidget::on_brushSizeChanged);
    Q_ASSERT(result);
}

void ToolSettingsWidget::on_brushSizeChanged(int size)
{
    if ( m_settings ) {
        m_settings->setBrushSize(size);
    }
}
