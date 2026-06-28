#ifndef ERASERTOOL_HPP
#define ERASERTOOL_HPP

#include "tool.hpp"

class EraserTool : public Tool
{
public:
    explicit EraserTool(QObject* parent = nullptr);

    void handleMousePress(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos) override;

private:
    void processDrawing(QPointF pos);

private:
    QList<QGraphicsItem*> m_erasedItems; //!< Для группировки удаления
};

#endif // ERASERTOOL_HPP
