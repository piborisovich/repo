#ifndef SIMPLESIZEGRAPHICSITEM_HPP
#define SIMPLESIZEGRAPHICSITEM_HPP

#include <QFont>
#include <QGraphicsItem>

class SimpleSizeGraphicsItem : public QGraphicsItem
{
public:
    SimpleSizeGraphicsItem(QGraphicsItem *parent = nullptr);
    ~SimpleSizeGraphicsItem();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    QPointF p1() const;
    void setP1(QPointF newP1);

    QPointF p2() const;
    void setP2(QPointF newP2);

    QString text() const;
    void setText(const QString &newText);

    int height() const;
    void setHeight(int newHeight);

    QFont font() const;
    void setFont(const QFont &newFont);

private:

    static const int    ARROW_HEIGHT;
    static const double ARROW_ANGLE;  //!< Угол стрелки, град.
    static const int    TEXT_SPACING; //!< Отступ текста от стрелки
    static const int    LINE_SPACING; //!< Отступ размерной линии от края выносок

    double rotationAngle();
    void initMarker(QPolygonF &poly);
    void recalc();


    int m_height;         //!< Высота выноски
    QLineF m_baseLine;    //!< Линия, соединяющая, указанные точки
    QLineF m_sizeLine;    //!< Размерная линия
    QLineF m_spacingLine; //! Точки для рисования выносок

    QRectF m_rect;
    QPolygonF m_poly;
    QFont m_font;
    QString m_text;

    QRectF textb;
};

#endif // SIMPLESIZEGRAPHICSITEM_HPP
