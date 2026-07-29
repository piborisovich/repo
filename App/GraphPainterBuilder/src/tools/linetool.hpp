#ifndef LINETOOL_HPP
#define LINETOOL_HPP

#include "tool.hpp"
#include "lineitem.hpp"

class LineTool : public Tool
{
public:
    explicit LineTool(QObject* parent = nullptr);

    void handleMousePress(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos) override;

private:
    void initLine(const QPointF &firstPoint);

private:
    LineItem *m_previewLine;
    QPointF m_startPoint;
};

#endif // LINETOOL_HPP
