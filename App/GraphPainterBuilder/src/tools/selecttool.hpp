#ifndef SELECTTOOL_HPP
#define SELECTTOOL_HPP

#include "tool.hpp"

class SelectTool : public Tool
{
public:
    explicit SelectTool(QObject* parent = nullptr);

    void handleMousePress(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos) override;
};

#endif // SELECTTOOL_HPP
