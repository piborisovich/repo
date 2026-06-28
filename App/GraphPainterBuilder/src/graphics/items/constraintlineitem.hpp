#ifndef CONSTRAINTLINEITEM_HPP
#define CONSTRAINTLINEITEM_HPP

#include <QGraphicsLineItem>

/*!
 * \brief Ограничительная линия
 */
class ConstraintLineItem : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    /*!
     * \brief ConstraintLineItem
     * \param dimensions - в засвисимости от ориентации:
     *        Qt::vertical: x - координата по Ox, y  - длина относительно (0, 0)
     *        Qt::horizontal: x - длина относително (0, 0), y  - координата по Oy
     * \param orientation
     * \param parent
     */
    ConstraintLineItem(const QPointF &point,
                       Qt::Orientations orientation = Qt::Vertical,
                       QGraphicsItem* parent = nullptr);

Q_SIGNALS:
    void positionChanged( const QPointF & point );
public:
    /*!
     * \brief Расширение зоны клика
     * \return
     */
    QPainterPath shape() const override;

    QRectF boundingRect() const override;

    Qt::Orientations orientation() const;

    QPointF point() const;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    Qt::Orientations m_orientation;
    QPointF m_point;
};

#endif // CONSTRAINTLINEITEM_HPP
