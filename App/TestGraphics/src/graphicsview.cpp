#include "graphicsview.hpp"

#include <QMouseEvent>


const QBrush GraphicsView::BACKGROUND_BRUSH = QBrush(QColor(255, 0, 0, 50));

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setScene(new QGraphicsScene(this));
    scene()->setBackgroundBrush(BACKGROUND_BRUSH);
    setMouseTracking(true);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    emit mousePressed(event->button(), mapToScene(event->pos()));
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseReleased(event->button(), mapToScene(event->pos()));
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMove(mapToScene(event->pos()));
}
