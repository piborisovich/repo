#include "graphicsview.hpp"

#include <QScrollBar>
#include <QWheelEvent>

static const qreal DEFAULT_SCALE_CHANGED_STEP_UP = 1.25;
static const qreal DEFAULT_SCALE_CHANGED_STEP_DOWN = 0.8;

const QBrush GraphicsView::DEFAULT_BACKGROUND_BRUSH = QBrush(QColor(230, 230, 230));

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
    , m_scene( new GraphicsScene() )
    , m_scale(1.0)
{
    setBackgroundBrush(DEFAULT_BACKGROUND_BRUSH);

    setScene(m_scene);
    setMouseTracking(true);
    setUpdatesEnabled(true);
    setRenderHint( QPainter::Antialiasing );

    setAlignment(Qt::AlignCenter);

    auto result = connect(m_scene,
                          &GraphicsScene::canUndoChanged,
                          this,
                          &GraphicsView::canUndoChanged);
    Q_ASSERT(result);

    result = connect(m_scene,
                     &GraphicsScene::canRedoChanged,
                     this,
                     &GraphicsView::canRedoChanged);
    Q_ASSERT(result);

    result = connect(m_scene,
                     &GraphicsScene::itemSelected,
                     this,
                     &GraphicsView::itemSelected);
    Q_ASSERT(result);

    result = connect(m_scene,
                     &GraphicsScene::itemDeselected,
                     this,
                     &GraphicsView::itemDeselected);
    Q_ASSERT(result);
}

GraphicsView::~GraphicsView()
{
    if ( m_scene ) {
        m_scene->clearSelection();
        m_scene->setFocusItem(nullptr);
        m_scene->clear();
    }
}

void GraphicsView::undo()
{
    m_scene->undo();
}

void GraphicsView::redo()
{
    m_scene->redo();
}

void GraphicsView::addSceneListener(ISceneListener *listener)
{
    m_scene->addSceneListener(listener);
}

void GraphicsView::removeSceneListener(ISceneListener *listener)
{
    m_scene->removeSceneListener(listener);
}

void GraphicsView::addImage(const QString &path, QSize &imageSize)
{
    m_scene->addImage(path, imageSize);
}

bool GraphicsView::save(const QString &filePath)
{
    QImage image(sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(filePath.endsWith(".png", Qt::CaseInsensitive) ? Qt::transparent : Qt::white);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    m_scene->render(&painter);
    painter.end();

    return image.save(filePath);
}

int GraphicsView::currentLayerZ() const
{
    return m_scene->currentLayerZ();
}

void GraphicsView::setCurrentLayerZ(int newCurrentLayerZ)
{
    m_scene->setCurrentLayerZ(newCurrentLayerZ);
}

void GraphicsView::setCurrentrColor(const QColor &color)
{
    m_scene->setCurrentColor(color);
}

QColor GraphicsView::currentColor() const
{
    return m_scene->currentColor();
}

void GraphicsView::clearLayer(int layerZ)
{
    m_scene->clearLayer(layerZ);

}

void GraphicsView::clear()
{
    m_scene->clearScene();
}

bool GraphicsView::isSceneEmpty() const
{
    return m_scene->items().isEmpty();
}

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);

    // Задаем цвет и заливку для рабочей области (sceneRect)
    painter->setBrush( m_scene->backgroundBrush() );
    painter->setPen(Qt::NoPen);

    // Рисуем прямоугольник размером с рабочую область
    painter->drawRect(sceneRect());
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if ( event->modifiers() == Qt::ControlModifier ) {

        QPointF scenePos = mapToScene(event->position().toPoint());

        if ( event->angleDelta().y() > 0 ) {
            m_scale *= DEFAULT_SCALE_CHANGED_STEP_UP;
            scale(DEFAULT_SCALE_CHANGED_STEP_UP,
                  DEFAULT_SCALE_CHANGED_STEP_UP);
        } else {
            m_scale *= DEFAULT_SCALE_CHANGED_STEP_DOWN;
            scale(DEFAULT_SCALE_CHANGED_STEP_DOWN,
                  DEFAULT_SCALE_CHANGED_STEP_DOWN);
        }

        setTransformationAnchor(GraphicsView::AnchorUnderMouse);
        centerOn(scenePos);

        emit scaleChanged(m_scale);
    }
    QGraphicsView::wheelEvent(event);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::MiddleButton ) {
        m_origin = event->pos();
    }
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if ( event->buttons() & Qt::MiddleButton ) {
        const QPointF move = m_origin - event->pos(); // The move
        horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
        verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());

        m_origin = event->pos();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}
