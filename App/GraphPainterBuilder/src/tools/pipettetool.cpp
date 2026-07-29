#include "pipettetool.hpp"

static const int CURSOR_WIDTH = 24;

PipetteTool::PipetteTool(QObject *parent) :
    Tool("Pipette",
         nullptr,
         ":/pipette.png",
         parent)
{

}

const QCursor &PipetteTool::getCursor() const
{
    static bool init = false;
    static QCursor cursor;

    if ( !init ) {

        QPixmap cursorPixmap(CURSOR_WIDTH, CURSOR_WIDTH);
        cursorPixmap.fill(Qt::transparent);
        auto w_div2 = CURSOR_WIDTH / 2;

        QPainter painter(&cursorPixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::black);

        // Рисуем форму курсора (круговой индикатор с перекрестием)
        painter.drawEllipse(2, 2, CURSOR_WIDTH - 4, CURSOR_WIDTH - 4);
        painter.drawLine(w_div2, 0, w_div2, CURSOR_WIDTH);
        painter.drawLine(0, w_div2, CURSOR_WIDTH, w_div2);
        painter.end();
        cursor = QCursor(cursorPixmap, w_div2, w_div2);

        init = true;
    }

    return cursor;
}

void PipetteTool::handleMousePress(Qt::MouseButton button, const QPointF &scenePos)
{
    if ( button == Qt::LeftButton ) {
        if ( scene() != nullptr ) {
            emit colorChangeRequested( getColor(scenePos) );
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
    handleMousePress( buttons & Qt::LeftButton ? Qt::LeftButton
                                              : Qt::NoButton, scenePos );
}


QColor PipetteTool::getColor(const QPointF &scenePos)
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
    return image.pixelColor(0, 0);
}
