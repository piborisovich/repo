#ifndef RECTANGLETOOL_HPP
#define RECTANGLETOOL_HPP

#include "tool.hpp"

class RectangleTool : public Tool
{
public:
    explicit RectangleTool(QObject* parent = nullptr);

    void handleMousePress(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos) override;

private:
    void processDrawing(QPointF pos);
};

#endif // RECTANGLETOOL_HPP
