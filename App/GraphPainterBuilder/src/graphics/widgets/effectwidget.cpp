#include "effectwidget.hpp"
#include "graphicspixelateeffect.hpp"
#include <QGraphicsBlurEffect>

template<class effectT>
EffectWidget<effectT>::EffectWidget(QWidget *parent)
    : BaseEffectWidget(parent)
{
}

template<class effectT>
EffectWidget<effectT>::~EffectWidget()
{
}

template<class effectT>
void EffectWidget<effectT>::on_apply()
{

}

template class EffectWidget<QGraphicsBlurEffect>;
template class EffectWidget<GraphicsPixelateEffect>;