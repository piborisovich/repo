#include "constraintlineitem.hpp"

#include <QCursor>
#include <QGraphicsScene>
#include <QPen>

ConstraintLineItem::ConstraintLineItem(const QPointF &point,
                                       Qt::Orientations orientation,
                                       QGraphicsItem *parent)
    : QObject()
    , QGraphicsLineItem( orientation == Qt::Vertical ? QLineF( point.x(), 0, point.x(), point.y() )
                                                     : QLineF( 0, point.y(), point.x(), point.y() ),parent)
    , m_orientation(orientation)
    , m_point(point)
{
    // Визуально линия остается тонкой и аккуратной
    QPen pen(Qt::red, 2, Qt::DashLine);
    setPen(pen);

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
}

QPainterPath ConstraintLineItem::shape() const
{
    QPainterPath path;

    path.moveTo(line().p1());
    path.lineTo(line().p2());

    // Создаем "толстый" контур вокруг нашей линии для обработки мыши
    QPainterPathStroker stroker;
    stroker.setWidth(6); // Ширина зоны захвата в пикселях (подберите под себя)
    stroker.setCapStyle(Qt::SquareCap); // Делаем углы зоны квадратными

    return stroker.createStroke(path);
}

QRectF ConstraintLineItem::boundingRect() const
{
    return shape().controlPointRect();
}

void ConstraintLineItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(m_orientation == Qt::Vertical ? Qt::SizeHorCursor : Qt::SizeVerCursor); //курсор меняется на всей широкой области
    QGraphicsLineItem::hoverEnterEvent(event);
}

void ConstraintLineItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    unsetCursor();
    QGraphicsLineItem::hoverLeaveEvent(event);
}

QVariant ConstraintLineItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if ( change == ItemPositionChange && scene() ) {

        QPointF newPos = value.toPointF();

        qreal min = 0.0;
        qreal max = 0.0;
        qreal current = 0.0;

        // Ограничения движения (в пределах сцены)
        if ( m_orientation == Qt::Vertical ) {
            newPos.setY(0); // Движение только по горизонтали
            max = scene()->width();
            current = line().x1() + newPos.x();

            if (current < min) {
                newPos.setX(min - line().x1());
                current = min;
            } else if (current > max) {
                newPos.setX(max - line().x1());
                current = max;
            }

            m_point = QPointF(current, 0);

        } else {
            newPos.setX(0); // Движение только по вертикали
            max = scene()->height();
            current = line().y1() + newPos.y();

            if (current < min) {
                newPos.setY(min - line().y1());
                current = min;
            } else if (current > max) {
                newPos.setY(max - line().y1());
                current = max;
            }

            m_point = QPointF(0, current);
        }

        emit positionChanged( m_point );

        return newPos;
    }
    return QGraphicsLineItem::itemChange(change, value);
}

QPointF ConstraintLineItem::point() const
{
    return m_point;
}

Qt::Orientations ConstraintLineItem::orientation() const
{
    return m_orientation;
}
