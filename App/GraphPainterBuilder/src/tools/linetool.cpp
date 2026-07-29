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
    if ( button == Qt::LeftButton ) {

        //Содержится ли вершина на сцене, включая толщину
        if ( vertexOnScene(scenePos) ) {
            initLine(scenePos);
        }
    }
}

void LineTool::handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(scenePos);

    GraphicsScene *gScene = qobject_cast<GraphicsScene*>(scene());

    if ( gScene == nullptr ) return;

    if ( button == Qt::LeftButton ) {
        if ( m_previewLine ) {
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

    if ( buttons & Qt::LeftButton ) {

        if ( m_previewLine && vertexOnScene(scenePos) ) {
            m_previewLine->setLine( QLineF (m_startPoint, scenePos ) );
        } else if ( m_previewLine == nullptr && vertexOnScene(scenePos) ) {
            initLine(scenePos);
        }
    }
}

void LineTool::initLine(const QPointF &firstPoint)
{
    GraphicsScene *gScene = scene();

    if ( gScene == nullptr ) return;

    m_startPoint = firstPoint;

    QPen pen(gScene->currentColor(),
             settings()->penSize(),
             Qt::SolidLine,
             Qt::RoundCap,
             Qt::RoundJoin);

    m_previewLine = new LineItem(QLineF(m_startPoint, m_startPoint));
    m_previewLine->setPen(pen);
    m_previewLine->setZValue(gScene->currentLayerZ());
    scene()->addItem(m_previewLine); // Временно добавляем для предпросмотра
}
