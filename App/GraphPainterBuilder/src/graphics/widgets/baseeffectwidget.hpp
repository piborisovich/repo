#ifndef BASEEFFECTWIDGET_HPP
#define BASEEFFECTWIDGET_HPP

#include <QAbstractButton>
#include <QWidget>

namespace Ui {
class BaseEffectWidget;
}

/*!
 * \brief Базовый класс эффектов (со слотами и сигналами)
 */
class BaseEffectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseEffectWidget(QWidget *parent = nullptr);
    ~BaseEffectWidget();

Q_SIGNALS:
    void apply(int size);

protected:
    virtual void on_apply() = 0;

private Q_SLOTS:
    void on_buttonClicked(QAbstractButton* button);

private:
    Ui::BaseEffectWidget *ui;
};

#endif // BASEEFFECTWIDGET_HPP
