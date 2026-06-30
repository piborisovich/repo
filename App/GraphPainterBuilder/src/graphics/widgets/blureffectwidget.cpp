#include "blureffectwidget.hpp"
#include "ui_blureffectwidget.h"

BlurEffectWidget::BlurEffectWidget(QWidget *parent)
    : QWidget(parent, Qt::Window)
    , ui(new Ui::BlurEffectWidget)
{
    ui->setupUi(this);
    setWindowTitle("Blur");

    QLabel* radiusValueLabel = ui->radiusValueLabel;

    auto result = connect(ui->radiusSlider,
                          &QSlider::sliderMoved,
                          this,
                          [radiusValueLabel](int value){
                              radiusValueLabel->setText(QString::number(value));
                          });
    Q_ASSERT(result);

    result = connect(ui->buttonBox,
                     &QDialogButtonBox::clicked,
                     this,
                     &BlurEffectWidget::on_buttonClicked);
    Q_ASSERT(result);
}

BlurEffectWidget::~BlurEffectWidget()
{
    delete ui;
}

void BlurEffectWidget::on_buttonClicked(QAbstractButton *button)
{
    auto standardButton = ui->buttonBox->standardButton(button);

    switch ( standardButton ) {
    case QDialogButtonBox::Cancel:
        close();
        break;
    case QDialogButtonBox::Ok:
        Q_EMIT applyRadius( ui->radiusSlider->value() );
        close();
        break;
    case QDialogButtonBox::Apply:
        Q_EMIT applyRadius( ui->radiusSlider->value() );
        break;
    case QDialogButtonBox::Discard:
        break;
    default:;
    }
}
