#include "baseeffectwidget.hpp"
#include "ui_baseeffectwidget.h"

#include <QDialogButtonBox>

BaseEffectWidget::BaseEffectWidget(const QPixmap &pixmap,
                                   QWidget *parent)
    : QWidget(parent, Qt::Dialog)
    , ui(new Ui::BaseEffectWidget)
{
    ui->setupUi(this);

    QLabel* radiusValueLabel = ui->radiusValueLabel;

    auto result = connect(ui->radiusSlider,
                          &QSlider::sliderMoved,
                          this,
                          [radiusValueLabel](int value){
                              radiusValueLabel->setText(QString::number(value));
                          });
    Q_ASSERT(result);

    result = connect(ui->radiusSlider,
                     &QSlider::sliderReleased,
                     this,
                     &BaseEffectWidget::on_sliderReleased);
    Q_ASSERT(result);

    result = connect(ui->buttonBox,
                     &QDialogButtonBox::clicked,
                     this,
                     &BaseEffectWidget::on_buttonClicked);
    Q_ASSERT(result);

    ui->effectExampleLabel->setPixmap( pixmap.scaledToWidth(width(), Qt::SmoothTransformation ) );
    ui->effectExampleLabel->setScaledContents(false);
    ui->effectExampleLabel->adjustSize();

    setFixedSize(size());
}

BaseEffectWidget::~BaseEffectWidget()
{
    delete ui;
}

void BaseEffectWidget::on_buttonClicked(QAbstractButton *button)
{
    auto standardButton = ui->buttonBox->standardButton(button);

    switch ( standardButton ) {
    case QDialogButtonBox::Cancel:
        close();
        break;
    case QDialogButtonBox::Ok:
        Q_EMIT apply(ui->radiusSlider->value());
        close();
        break;
    case QDialogButtonBox::Apply:
        Q_EMIT apply(ui->radiusSlider->value());
        break;
    case QDialogButtonBox::Discard:
        break;
    default:;
    }
}

void BaseEffectWidget::on_sliderReleased()
{
    ui->effectExampleLabel->setGraphicsEffect(createEffect(ui->radiusSlider->value()));
}

