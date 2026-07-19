#ifndef EFFECTWIDGET_HPP
#define EFFECTWIDGET_HPP

#include "baseeffectwidget.hpp"

template<class effectT>
class EffectWidget : public BaseEffectWidget
{
public:
    explicit EffectWidget(const QPixmap &pixmap,
                          QWidget *parent = nullptr);
    ~EffectWidget();

protected:
    QGraphicsEffect* createEffect(int size) override;
};

#endif // EFFECTWIDGET_HPP
