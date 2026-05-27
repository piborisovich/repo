#include "toolswidget.hpp"

#include "strings.hpp"

#include <QLabel>
#include <QVBoxLayout>
#include <QColorDialog>
#include <QComboBox>

ToolsWidget::ToolsWidget(const QString &title, const TColorGetter &colorGetter,
                         QWidget *parent,
                         Qt::WindowFlags flags) : QDockWidget(title, parent, flags)
    , m_colorButton(new QPushButton())
    , m_colorGetter(colorGetter)
{
    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *widgetLayout = new QVBoxLayout(contentWidget);

    // Выбор инструмента
    widgetLayout->addWidget( new QLabel(Strings::TOOL_TEXT) );
    QComboBox *toolBox = new QComboBox();
    toolBox->addItems( {"Select", "Drag", "Brush", "Eraser", "Line", "Rectangle", "Circle"} );
    widgetLayout->addWidget(toolBox);

    // Настройка размера
    widgetLayout->addWidget(new QLabel(Strings::BRUSH_SIZE_TEXT));
    QSlider *sizeSlider = new QSlider(Qt::Horizontal);
    sizeSlider->setRange(1, 100);
    sizeSlider->setValue(5);
    widgetLayout->addWidget(sizeSlider);

    // Выбор цвета
    widgetLayout->addWidget(new QLabel(Strings::PALETTE_TEXT));
    updateColorButtonLayout(Qt::black);
    widgetLayout->addWidget(m_colorButton);

    // Экспорт холста в файл
    widgetLayout->addSpacing(15);

    // Кнопка полной очистки холста
    widgetLayout->addSpacing(10);

    widgetLayout->addStretch();

    setWidget(contentWidget);


    auto result = connect(toolBox,
                          &QComboBox::currentTextChanged,
                          this,
                          &ToolsWidget::currentToolChanged);
    Q_ASSERT(result);

    result = connect(sizeSlider,
                     &QSlider::valueChanged,
                     this,
                     &ToolsWidget::brushSizeChanged);
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

void ToolsWidget::updateColorButtonLayout(QColor color)
{
    m_colorButton->setStyleSheet(QString("background-color: %1;"
                                         "min-height: 30px;"
                                         "border: 1px solid #555;").arg(color.name()));
}
