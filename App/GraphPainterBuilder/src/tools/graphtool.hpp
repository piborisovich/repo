#ifndef GRAPHTOOL_HPP
#define GRAPHTOOL_HPP

#include "tool.hpp"

class GraphTool : public Tool
{
public:
    explicit GraphTool(QObject* parent = nullptr);

    void handleMousePress(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos) override;

private:
    QPointF m_startPoint;
    QGraphicsRectItem *m_tmpRect;

};

#endif // GRAPHTOOL_HPP
