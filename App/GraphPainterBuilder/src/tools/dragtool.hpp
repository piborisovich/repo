#ifndef DRAGTOOL_HPP
#define DRAGTOOL_HPP

#include "tool.hpp"

class DragTool : public Tool
{
public:
    explicit DragTool(QObject* parent = nullptr);

    const QCursor &getCursor() const override;

    void handleMousePress(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos) override;
};

#endif // DRAGTOOL_HPP
