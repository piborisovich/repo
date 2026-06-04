#include "graphtool.hpp"
#include "graphicsscene.hpp"
#include "graphtoolsettings.hpp"
#include "commands.hpp"

#include <QGraphicsRectItem>

GraphTool::GraphTool(QObject* parent)
    : Tool("Graph",
           std::make_shared<GraphToolSettings>(),
           ":/graph.png",
           parent)
    , m_tmpRect(nullptr)
{
}

void GraphTool::handleMousePress(Qt::MouseButton button, const QPointF &scenePos)
{
    GraphicsScene *gScene = qobject_cast<GraphicsScene*>(scene());

    if ( gScene == nullptr ) return;

    if ( button == Qt::LeftButton ) {
        m_startPoint = scenePos;
        //m_erasedItemsThisStroke.clear();

        QPen pen(gScene->currentColor(),
                 settings()->brushSize(),
                 Qt::DashLine,
                 Qt::RoundCap,
                 Qt::RoundJoin);

        m_tmpRect = new QGraphicsRectItem(QRectF(m_startPoint, m_startPoint));

        m_tmpRect->setPen(pen);
        m_tmpRect->setZValue(gScene->currentLayerZ());

        scene()->addItem(m_tmpRect);
    }
}

void GraphTool::handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos)
{
    GraphicsScene *gScene = qobject_cast<GraphicsScene*>(scene());

    if ( gScene == nullptr ) return;
    if ( button == Qt::LeftButton ) {
        QRectF rect = m_tmpRect->rect();
        QList<QPointF> points;

        QPen pen(gScene->currentColor(),
                 settings()->brushSize(),
                 Qt::SolidLine,
                 Qt::RoundCap,
                 Qt::RoundJoin);

        scene()->removeItem(m_tmpRect);
        m_tmpRect = nullptr;

        qreal x0 = rect.left();
        qreal step = rect.width() / 100;
        for (int i = 0; i < 100; ++i ) {
            QPointF point (x0, rect.top() + (rand() / qreal(RAND_MAX) * rect.height()));
            points.push_back(point);
            x0 += step;

            qDebug() << point;
        }

        QPainterPath painterPath;

        painterPath.addPolygon(points);
        auto item = scene()->addPath(painterPath, pen);

        gScene->addSceneCommand(new Commands::AddItemCommand(scene(),item));
    }
}

void GraphTool::handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos)
{
    if ( buttons & Qt::LeftButton ) {
        QPointF leftPoint(qMin(scenePos.x(), m_startPoint.x()), qMin(scenePos.y(), m_startPoint.y()));
        QPointF rightPoint(qMax(scenePos.x(), m_startPoint.x()), qMax(scenePos.y(), m_startPoint.y()));

        m_tmpRect->setRect( QRectF(leftPoint,
                                  rightPoint));
    }
}
