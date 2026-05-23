#ifndef ISCENELISTENER_HPP
#define ISCENELISTENER_HPP

#include <QPointF>

/*!
 * \brief Интерфейс наблюдателя за сценой
 */
class ISceneListener
{
public:

    virtual void mousePressed(Qt::MouseButton button, const QPointF &scenePos) = 0;
    virtual void mouseReleased(Qt::MouseButton button, const QPointF &scenePos) = 0;
    virtual void mouseMoved(const QPointF &scenePos) = 0;

    virtual ~ISceneListener() {}


};

#endif // ISCENELISTENER_HPP
