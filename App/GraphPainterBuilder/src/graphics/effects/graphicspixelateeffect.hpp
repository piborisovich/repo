#ifndef GRAPHICSPIXELATEEFFECT_HPP
#define GRAPHICSPIXELATEEFFECT_HPP

#include <QGraphicsEffect>

/*!
 * \brief Эффект пикселизации
 */
class GraphicsPixelateEffect : public QGraphicsEffect
{
    Q_OBJECT
    Q_PROPERTY(int pixelSize READ pixelSize WRITE setPixelSize)

public:
    explicit GraphicsPixelateEffect(QObject *parent = nullptr);

    int pixelSize() const;

    void setPixelSize(int size);

protected:
    void draw(QPainter *painter) override;

private:
    int m_pixelSize;
};

#endif // GRAPHICSPIXELATEEFFECT_HPP
