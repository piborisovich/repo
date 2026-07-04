#ifndef EFFECTWIDGET_HPP
#define EFFECTWIDGET_HPP

#include "baseeffectwidget.hpp"

template<class effectT>
class EffectWidget : public BaseEffectWidget
{
public:
    explicit EffectWidget(QWidget *parent = nullptr);
    ~EffectWidget();

protected:
    void on_apply() override;
};

#endif // EFFECTWIDGET_HPP
