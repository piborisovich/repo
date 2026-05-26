#ifndef ITOOL_HPP
#define ITOOL_HPP

#include <QPointF>
#include <QCursor>

/*!
 * \brief Интерфейс инструмента
 */
class ITool
{
public:
     virtual ~ITool() {}
protected:
    virtual void handleMousePress(Qt::MouseButton button, const QPointF &scenePos) = 0;
    virtual void handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos) = 0;
    virtual void handleMouseMove(Qt::MouseButton button, const QPointF &scenePos) = 0;
    virtual QCursor getCursor() = 0;
};

#endif // ITOOL_HPP
