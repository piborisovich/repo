#ifndef BRUSHTOOL_HPP
#define BRUSHTOOL_HPP

#include "tool.hpp"

/*!
 * \brief Кисть
 */
class BrushTool : public Tool
{
public:
    explicit BrushTool(QObject* parent = nullptr);

    void handleMousePress(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos) override;

private:
    void processDrawing(QPointF pos);

private:
    QList<QGraphicsItem*> m_brushItems;

};

#endif // BRUSHTOOL_HPP
