#include "pipettetool.hpp"

PipetteTool::PipetteTool(QObject *parent) :
    Tool("Pipette",
         nullptr,
         ":/pipette.png",
         parent)
{

}

void PipetteTool::handleMousePress(Qt::MouseButton button, const QPointF &scenePos)
{
    if ( button == Qt::LeftButton ) {
        if ( scene() != nullptr ) {
            pickColor(scenePos);
        }
    }
}

void PipetteTool::handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(button);
    Q_UNUSED(scenePos);
}

void PipetteTool::handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos)
{
    if ( buttons & Qt::LeftButton ) {
        pickColor(scenePos);
    }
}


void PipetteTool::pickColor(const QPointF &scenePos)
{
    // Задаем область 1 x 1 вокруг курсора
    QRectF sceneRect(scenePos.x(), scenePos.y(), 1, 1);
    QRect targetRect(0, 0, 1, 1);

    // Создаем QImage формата ARGB для сохранения качества
    QImage image(targetRect.size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    // Рендерим нужную часть сцены в image
    QPainter painter(&image);
    scene()->render(&painter, targetRect, sceneRect);
    painter.end();

    // Получаем цвет
    QColor color = image.pixelColor(0, 0);

    emit colorChangeRequested(color);
}
