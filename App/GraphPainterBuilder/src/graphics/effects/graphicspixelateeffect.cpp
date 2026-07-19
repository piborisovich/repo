#include "graphicspixelateeffect.hpp"

#include <QPainter>
#include <QPoint>

GraphicsPixelateEffect::GraphicsPixelateEffect(QObject *parent)
    : QGraphicsEffect(parent)
    , m_pixelSize(1)
{

}

int GraphicsPixelateEffect::pixelSize() const
{
    return m_pixelSize;
}

void GraphicsPixelateEffect::setPixelSize(int size)
{
    if (m_pixelSize != size) {
        m_pixelSize = size;
        update(); // Перерисовываем эффект при изменении размера пикселей
    }
}

void GraphicsPixelateEffect::draw(QPainter *painter)
{
    QPoint offset;
    QPixmap source = sourcePixmap(Qt::LogicalCoordinates, &offset);
    if (source.isNull()) return;

    // Определяем область для пикселизации
    QRect rect = source.rect();

    auto px = m_pixelSize >= 1 ? m_pixelSize : 1;

    // Уменьшаем изображение до размера блоков
    QSize smallSize = QSize(rect.width() / px, rect.height() / px);
    if (smallSize.isEmpty()) return;

    QImage smallImage = source.toImage().scaled(smallSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    // Увеличиваем обратно для получения пиксельного эффекта
    QImage pixelatedImage = smallImage.scaled(rect.size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);

    // Рисуем полученное изображение
    painter->drawPixmap(offset, QPixmap::fromImage(pixelatedImage));
}
