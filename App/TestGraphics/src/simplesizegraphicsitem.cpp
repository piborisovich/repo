#include "simplesizegraphicsitem.hpp"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

const int SimpleSizeGraphicsItem::ARROW_HEIGHT   = 20;
const double SimpleSizeGraphicsItem::ARROW_ANGLE = 20;
const int SimpleSizeGraphicsItem::TEXT_SPACING   = 5;
const int SimpleSizeGraphicsItem::LINE_SPACING   = 5;

SimpleSizeGraphicsItem::SimpleSizeGraphicsItem(QGraphicsItem *parent) : QGraphicsItem(parent),
    m_height(0)
{
    initMarker(m_poly);
}

SimpleSizeGraphicsItem::~SimpleSizeGraphicsItem()
{
}

QRectF SimpleSizeGraphicsItem::boundingRect() const
{
    return m_rect;
}

void SimpleSizeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    double angle = rotationAngle();

    painter->save();
    painter->setPen(Qt::SolidLine);

    if ( m_height ) {
        painter->drawLine(m_baseLine.p1(), m_spacingLine.p1());
        painter->drawLine(m_baseLine.p2(), m_spacingLine.p2());
    }

    painter->drawLine(m_sizeLine);

    painter->setBrush(QBrush(Qt::black));
    painter->drawPolygon(QTransform().rotate(-m_sizeLine.angle() - 90).map(m_poly).translated(m_sizeLine.p1()));
    painter->drawPolygon(QTransform().rotate(-m_sizeLine.angle() + 90).map(m_poly).translated(m_sizeLine.p2()));

    m_text = QString::number(m_sizeLine.angle());
    QFontMetrics fm(font());
    QRectF textRect = fm.boundingRect(m_text);

    painter->translate(m_sizeLine.center());
    painter->rotate(angle);

    painter->drawText(QPointF(0, -TEXT_SPACING) + QPointF(-textRect.width() / 2, 0), m_text);

    painter->rotate(-angle);
    painter->translate(-m_sizeLine.center());

    painter->setBrush(QBrush(Qt::NoBrush));
    painter->drawRect(boundingRect());
    painter->drawRect(textb);

    painter->restore();
}

QPointF SimpleSizeGraphicsItem::p1() const
{
    return m_baseLine.p1();
}

void SimpleSizeGraphicsItem::setP1(QPointF newP1)
{
    m_baseLine.setP1(newP1);
    recalc();
}

QPointF SimpleSizeGraphicsItem::p2() const
{
    return m_baseLine.p2();
}

void SimpleSizeGraphicsItem::setP2(QPointF newP2)
{
    m_baseLine.setP2(newP2);
    recalc();
}

void SimpleSizeGraphicsItem::initMarker(QPolygonF &poly)
{
    double dx = tan(qDegreesToRadians(ARROW_ANGLE / 2.0)) * ARROW_HEIGHT;
    poly.clear();
    poly << QPointF(0, 0);
    poly << QPointF(dx, ARROW_HEIGHT);
    poly << QPointF(0, ARROW_HEIGHT * 0.9);
    poly << QPointF(-dx, ARROW_HEIGHT);
}

void SimpleSizeGraphicsItem::recalc()
{
    QPolygonF poly = QPolygonF() << m_baseLine.p1() << m_baseLine.p2();
    double angle = rotationAngle();
    double angle_rad = qDegreesToRadians(90 + angle);
    double cos_a = cos(angle_rad);
    double sin_a = sin(angle_rad);

    QFontMetrics fm(font());

    QRectF fontRect = fm.boundingRect(m_text);
    double textTranslateLen = fontRect.height() / 2 + TEXT_SPACING;

    m_sizeLine = m_baseLine;

    if ( m_height ) {

        int spacing = (m_height > 0) ? LINE_SPACING : -LINE_SPACING;

        m_sizeLine = m_baseLine.translated(-m_height * cos_a, -m_height * sin_a);
        m_spacingLine = m_sizeLine.translated(-spacing * cos_a, -spacing * sin_a);

        poly << m_spacingLine.p1() << m_spacingLine.p2();
    }

    fontRect = QTransform().rotate(angle).mapRect(fontRect.translated(-fontRect.center())).translated(m_sizeLine.center() - QPointF(textTranslateLen * cos_a, textTranslateLen * sin_a));

    textb = fontRect;

    poly << QTransform().rotate(-m_baseLine.angle() - 90).map(m_poly).translated(m_sizeLine.p1());
    poly << QTransform().rotate(-m_baseLine.angle() + 90).map(m_poly).translated(m_sizeLine.p2());

    m_rect = poly.boundingRect().united(fontRect).adjusted(-1, -1, 1, 1); //С учетом толщин линий
}

QFont SimpleSizeGraphicsItem::font() const
{
    return m_font;
}

void SimpleSizeGraphicsItem::setFont(const QFont &newFont)
{
    m_font = newFont;
    recalc();
}

int SimpleSizeGraphicsItem::height() const
{
    return m_height;
}

void SimpleSizeGraphicsItem::setHeight(int newHeight)
{
    m_height = newHeight;
    recalc();
}

inline double SimpleSizeGraphicsItem::rotationAngle()
{
    double div = m_baseLine.dy() / m_baseLine.dx();
    return ( div > 0 ? qRadiansToDegrees(atan(abs(div))) : -qRadiansToDegrees(atan(abs(div))) ) ;
}

QString SimpleSizeGraphicsItem::text() const
{
    return m_text;
}

void SimpleSizeGraphicsItem::setText(const QString &newText)
{
    m_text = newText;
    recalc();
}
