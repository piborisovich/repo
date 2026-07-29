#ifndef PIPETTETOOL_HPP
#define PIPETTETOOL_HPP

#include "tool.hpp"

class PipetteTool : public Tool
{
public:
    PipetteTool(QObject* parent = nullptr);

    const QCursor &getCursor() const override;

    void handleMousePress(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos) override;

private:
    QColor getColor(const QPointF &scenePos);
};

#endif // PIPETTETOOL_HPP
