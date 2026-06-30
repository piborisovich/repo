#ifndef BLUREFFECTWIDGET_HPP
#define BLUREFFECTWIDGET_HPP

#include <QAbstractButton>
#include <QWidget>

namespace Ui {
class BlurEffectWidget;
}

class BlurEffectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BlurEffectWidget(QWidget *parent = nullptr);
    ~BlurEffectWidget();

Q_SIGNALS:
    void applyRadius(int radius);

private Q_SLOTS:
    void on_buttonClicked(QAbstractButton* button);

private:
    Ui::BlurEffectWidget *ui;
};

#endif // BLUREFFECTWIDGET_HPP
