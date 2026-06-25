#include "graphtool.hpp"
#include "graphicsscene.hpp"
#include "graphtoolsettings.hpp"
#include "graphtoolsettingswidget.hpp"
#include "additemcommand.hpp"
#include "pathitem.hpp"

#include <QGraphicsDropShadowEffect>

GraphTool::GraphTool(QObject* parent)
    : Tool("Graph",
           std::make_shared<GraphToolSettings>(),
           ":/graph.png",
           parent)
    , m_xCursors(std::make_pair(nullptr, nullptr))
    , m_yCursors(std::make_pair(nullptr, nullptr))
{
    auto wdg = qobject_cast<GraphToolSettingsWidget*>(settingsWidget());

    if ( wdg ) {
        auto result = connect(wdg,
                              &GraphToolSettingsWidget::startPlot,
                              this,
                              &GraphTool::on_startPlot);
        Q_ASSERT(result);
    }
}

void GraphTool::handleMousePress(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(button);
    Q_UNUSED(scenePos);
}

void GraphTool::handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(button);
    Q_UNUSED(scenePos);
}

void GraphTool::handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos)
{
    Q_UNUSED(buttons);
    Q_UNUSED(scenePos);
}

void GraphTool::select()
{
    auto gScene = qobject_cast<GraphicsScene*>(scene());
    auto graphSettings = dynamic_cast<GraphToolSettings*>(settings().get());

    if ( graphSettings == nullptr || gScene == nullptr ) return;

    auto bRect = graphSettings->boundingRect();

    m_xCursors = std::make_pair( new ConstraintLineItem( QPointF(bRect.left(), scene()->height()) ),
                                 new ConstraintLineItem( QPointF(bRect.right(), scene()->height()) ) );

    m_yCursors = std::make_pair( new ConstraintLineItem(QPointF(scene()->width(), bRect.top()), Qt::Horizontal),
                                 new ConstraintLineItem(QPointF(scene()->width(), bRect.bottom()), Qt::Horizontal));


    m_xCursors.first->setZValue(gScene->currentLayerZ());
    m_xCursors.second->setZValue(gScene->currentLayerZ());

    m_yCursors.first->setZValue(gScene->currentLayerZ());
    m_yCursors.second->setZValue(gScene->currentLayerZ());

    scene()->addItem(m_xCursors.first);
    scene()->addItem(m_xCursors.second);

    scene()->addItem(m_yCursors.first);
    scene()->addItem(m_yCursors.second);


    auto result = connect(m_xCursors.first,
                          &ConstraintLineItem::positionChanged,
                          this,
                          &GraphTool::on_cursorPositionChanged);
    Q_ASSERT(result);

    result = connect(m_xCursors.second,
                     &ConstraintLineItem::positionChanged,
                     this,
                     &GraphTool::on_cursorPositionChanged);
    Q_ASSERT(result);

    result = connect(m_yCursors.first,
                     &ConstraintLineItem::positionChanged,
                     this,
                     &GraphTool::on_cursorPositionChanged);
    Q_ASSERT(result);

    result = connect(m_yCursors.second,
                     &ConstraintLineItem::positionChanged,
                     this,
                     &GraphTool::on_cursorPositionChanged);
    Q_ASSERT(result);

}

void GraphTool::unselect()
{
    removeFromScene();

    delete m_xCursors.first;
    delete m_xCursors.second;

    delete m_yCursors.first;
    delete m_yCursors.second;

    m_xCursors.first = nullptr;
    m_xCursors.second = nullptr;
    m_yCursors.first = nullptr;
    m_yCursors.second = nullptr;
}

void GraphTool::on_cursorPositionChanged(const QPointF &point)
{
    Q_UNUSED(point);

    auto graphSettings = dynamic_cast<GraphToolSettings*>(settings().get());

    if ( graphSettings != nullptr ) {
        qreal minX = m_xCursors.first->point().x();
        qreal maxX = m_xCursors.second->point().x();
        qreal minY = m_yCursors.first->point().y();
        qreal maxY = m_yCursors.second->point().y();;

        if (minX > maxX) {
            qSwap(minX, maxX);
        }

        if (minY > maxY) {
            qSwap(minY, maxY);
        }

        graphSettings->setBoundingRect( QRectF( QPointF(minX, minY),
                                                QPointF(maxX, maxY) ) );

    }
}

void GraphTool::on_startPlot()
{
    auto gScene = qobject_cast<GraphicsScene*>(scene());
    auto graphSettings = dynamic_cast<GraphToolSettings*>(settings().get());

    if ( gScene == nullptr ) return;
    if ( graphSettings == nullptr) return;

    QRectF rect = graphSettings->boundingRect();
    QList<QPointF> points;

    QPen pen(gScene->currentColor(),
             graphSettings->brushSize(),
             Qt::SolidLine,
             Qt::RoundCap,
             Qt::RoundJoin);

    qreal x0 = rect.left();

    qreal step = rect.width() / graphSettings->pointsCount();

    for (int i = 0; i <= graphSettings->pointsCount(); ++i ) {
        QPointF point (x0, rect.top() + (rand() / qreal(RAND_MAX) * rect.height()));
        points.push_back(point);
        x0 += step;
    }

    QPainterPath painterPath;

    painterPath.addPolygon(points);

    auto item = new PathItem(painterPath);
    item->setPen(pen);
    item->setZValue(gScene->currentLayerZ());

    gScene->addSceneCommand(new Commands::AddItemCommand(scene(),item));
}

void GraphTool::removeFromScene()
{
    if ( scene() ) {

        if ( m_xCursors.first && m_xCursors.first->scene() )scene()->removeItem(m_xCursors.first);
        if ( m_xCursors.second && m_xCursors.second->scene() )scene()->removeItem(m_xCursors.second);
        if ( m_yCursors.first && m_yCursors.first->scene())scene()->removeItem(m_yCursors.first);
        if ( m_yCursors.second && m_yCursors.second->scene()) scene()->removeItem(m_yCursors.second);
    }
}
