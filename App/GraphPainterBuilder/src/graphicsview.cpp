#include "graphicsview.hpp"


static const QRectF DEFAULT_SCENE_RECT( QPointF(0,0), QSizeF(800, 600) );

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
    , m_undoStack( new QUndoStack(this) )
    , m_scene( new GraphicsScene(m_undoStack, this) )
{
    setScene(m_scene);
    setMouseTracking(true);
    setUpdatesEnabled(true);
    setSceneRect( DEFAULT_SCENE_RECT );
    setRenderHint( QPainter::Antialiasing );

    auto result = connect(m_undoStack,
                          &QUndoStack::canUndoChanged,
                          this,
                          &GraphicsView::canUndoChanged);
    Q_ASSERT(result);

    result = connect(m_undoStack,
                     &QUndoStack::canRedoChanged,
                     this,
                     &GraphicsView::canRedoChanged);
    Q_ASSERT(result);
}

void GraphicsView::undo()
{
    m_undoStack->undo();
}

void GraphicsView::redo()
{
    m_undoStack->redo();
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
    QRectF sceneRect = m_scene->sceneRect();
    QImage image(sceneRect.size().toSize(), QImage::Format_ARGB32);
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

QString GraphicsView::currentTool() const
{
    return m_scene->currentTool();
}

void GraphicsView::setCurrentTool(const QString &newCurrentTool)
{
    m_scene->setCurrentTool(newCurrentTool);
}

int GraphicsView::brushSize() const
{
    return m_scene->brushSize();
}

void GraphicsView::setBrushSize(int newBrushSize)
{
    m_scene->setBrushSize(newBrushSize);
}

QColor GraphicsView::currentColor() const
{
    return m_scene->currentColor();
}

void GraphicsView::setCurrentColor(const QColor &newCurrentColor)
{
    m_scene->setCurrentColor(newCurrentColor);
}

void GraphicsView::clearLayer(int layerZ)
{
    QList<QGraphicsItem*> allItems = m_scene->items();
    for (QGraphicsItem *item : allItems) {
        if (item->zValue() == layerZ) {
            m_scene->removeItem(item);
            delete item;
        }
    }
    m_undoStack->clear(); // Очищаем историю, так как элементы физически удалены
}

void GraphicsView::clear()
{
    m_scene->clear();
    m_undoStack->clear();
}

bool GraphicsView::isSceneEmpty() const
{
    return m_scene->items().isEmpty();
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    QGraphicsView::wheelEvent(event);
}
