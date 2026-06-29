#include "itempropertieswidget.hpp"
#include "qpen.h"
#include "strings.hpp"

#include <QColorDialog>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QToolTip>

ItemPropertiesWidget::ItemPropertiesWidget(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , m_penColorButton(new QPushButton())
    , m_penSizeSlider( new QSlider( Qt::Horizontal) )
{
    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
    QGroupBox *penGroup = new QGroupBox("Pen");

    QVBoxLayout* penLayout = new QVBoxLayout(penGroup);

    m_penSizeSlider->setRange(1, 100);
    m_penSizeSlider->setValue(0);

    penLayout->addWidget(m_penColorButton);
    penLayout->addWidget(m_penSizeSlider);

    vBoxLayout->addWidget(penGroup);

    penLayout->setSpacing(5);
    vBoxLayout->setSpacing(5);

    penLayout->setContentsMargins(5, 5, 5, 5);
    vBoxLayout->setContentsMargins(5, 5, 5, 5);

    auto result = connect(m_penColorButton,
                          &QPushButton::clicked,
                          this,
                          &ItemPropertiesWidget::on_penColorClicked);
    Q_ASSERT(result);

    result = connect(m_penSizeSlider,
                     &QSlider::sliderMoved,
                     this,
                     &ItemPropertiesWidget::on_showSliderTooltip);
    Q_ASSERT(result);
}

void ItemPropertiesWidget::setPen(const QPen &pen)
{
    m_penColorButton->setStyleSheet(QString("background-color: %1;"
                                            "min-height: 30px;"
                                            "border: 1px solid #555;").arg( pen.color().name()) );

    m_penSizeSlider->setValue(pen.width());
}

QPen ItemPropertiesWidget::pen() const
{
    return QPen( m_penColorButton->palette().button().color(), m_penSizeSlider->value() );
}

void ItemPropertiesWidget::on_penColorClicked()
{
    QColor color = QColorDialog::getColor(m_penColorButton->palette().button().color(),
                                          this,
                                          Strings::SELECT_COLOR_TEXT);
    if (color.isValid()) {

        auto currentPen = pen();
        currentPen.setColor(color);
        setPen(currentPen);

        Q_EMIT penChanged(currentPen);
    }
}

void ItemPropertiesWidget::on_showSliderTooltip(int value)
{
    // Show the tooltip text instantly at the current cursor position
    QToolTip::showText(QCursor::pos(), QString::number(value), m_penSizeSlider);

    auto currentPen = pen();
    currentPen.setWidth(value);

    Q_EMIT penChanged(currentPen);
}
