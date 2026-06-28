#ifndef CIRCLETOOL_HPP
#define CIRCLETOOL_HPP

#include "tool.hpp"

class CircleTool : public Tool
{
public:
    explicit CircleTool(QObject* parent = nullptr);

    void handleMousePress(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos) override;

private:
    void processDrawing(QPointF pos);

};

#endif // CIRCLETOOL_HPP
