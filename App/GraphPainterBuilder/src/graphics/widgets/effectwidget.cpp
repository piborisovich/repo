#include "effectwidget.hpp"
#include "graphicspixelateeffect.hpp"
#include <QGraphicsBlurEffect>

template<class effectT>
EffectWidget<effectT>::EffectWidget(const QPixmap &pixmap,
                                    QWidget *parent)
    : BaseEffectWidget(pixmap, parent)
{
}

template<class effectT>
EffectWidget<effectT>::~EffectWidget()
{
}

template<class effectT>
QGraphicsEffect *EffectWidget<effectT>::createEffect(int size)
{
    Q_UNUSED(size);
    return new effectT();
}

template<>
QGraphicsEffect *EffectWidget<QGraphicsBlurEffect>::createEffect(int size)
{
    auto graphicsEffect = new QGraphicsBlurEffect();

    graphicsEffect->setBlurRadius(size);

    return graphicsEffect;
}

template<>
QGraphicsEffect *EffectWidget<GraphicsPixelateEffect>::createEffect(int size)
{
    auto graphicsEffect = new GraphicsPixelateEffect();

    graphicsEffect->setPixelSize(size);

    return graphicsEffect;
}

template class EffectWidget<QGraphicsBlurEffect>;
template class EffectWidget<GraphicsPixelateEffect>;