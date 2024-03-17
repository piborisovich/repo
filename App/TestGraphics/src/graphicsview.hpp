#ifndef GRAPHICSVIEW_HPP
#define GRAPHICSVIEW_HPP

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = nullptr);

signals:
    void mouseMove(const QPointF &scenePos);
    void mousePressed(Qt::MouseButton button, const QPointF &scenePos);
    void mouseReleased(Qt::MouseButton button, const QPointF &scenePos);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    static const QBrush BACKGROUND_BRUSH;
};

#endif // GRAPHICSVIEW_HPP
