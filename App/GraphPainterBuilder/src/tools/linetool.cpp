#include "linetool.hpp"
#include "graphicsscene.hpp"
#include "additemcommand.hpp"


LineTool::LineTool(QObject *parent) :
    Tool("Line",
         std::make_shared<ToolSettings>(),
         ":/line.png",
         parent)
    , m_previewLine(nullptr)
{

}

void LineTool::handleMousePress(Qt::MouseButton button, const QPointF &scenePos)
{
    GraphicsScene *gScene = scene();

    if ( gScene == nullptr ) return;

    if ( button == Qt::LeftButton ) {
        m_startPoint = scenePos;
        //m_erasedItemsThisStroke.clear();

        QPen pen(gScene->currentColor(),
                 settings()->brushSize(),
                 Qt::SolidLine,
                 Qt::RoundCap,
                 Qt::RoundJoin);

        m_previewLine = new LineItem(QLineF(m_startPoint, m_startPoint));
        m_previewLine->setPen(pen);
        m_previewLine->setZValue(gScene->currentLayerZ());
        scene()->addItem(m_previewLine); // Временно добавляем для предпросмотра
    }
}

void LineTool::handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(scenePos);

    GraphicsScene *gScene = qobject_cast<GraphicsScene*>(scene());

    if ( gScene == nullptr ) return;

    if ( button == Qt::LeftButton ) {
        if ( m_previewLine) {
            // Передаем созданную линию под управление UndoStack
            gScene->removeItem(m_previewLine);
            gScene->addSceneCommand( new Commands::AddItemCommand( scene(), m_previewLine ) );
            m_previewLine = nullptr;
        }
    }
}

void LineTool::handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos)
{
    Q_UNUSED(buttons);

    if ( m_previewLine ) {
        m_previewLine->setLine( QLineF (m_startPoint, scenePos ) );
    }
}
