#include "graphtool.hpp"
#include "graphicsscene.hpp"
#include "graphtoolsettings.hpp"
#include "graphtoolsettingswidget.hpp"
#include "commands.hpp"

#include <QGraphicsRectItem>

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

GraphTool::~GraphTool()
{
    removeFromScene();
}

void GraphTool::handleMousePress(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(button);
    Q_UNUSED(scenePos);
    /*GraphicsScene *gScene = qobject_cast<GraphicsScene*>(scene());

    if ( gScene == nullptr ) return;

    if ( button == Qt::LeftButton ) {
        m_startPoint = scenePos;
        //m_erasedItemsThisStroke.clear();

        QPen pen(QColor(Qt::gray),
                 1,
                 Qt::DashLine,
                 Qt::RoundCap,
                 Qt::RoundJoin);

        m_tmpRect = new QGraphicsRectItem(QRectF(m_startPoint, m_startPoint));

        m_tmpRect->setPen(pen);
        m_tmpRect->setZValue(gScene->currentLayerZ());

        scene()->addItem(m_tmpRect);
    }*/
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
    /*if ( buttons & Qt::LeftButton ) {
        QPointF leftPoint(qMin(scenePos.x(), m_startPoint.x()), qMin(scenePos.y(), m_startPoint.y()));
        QPointF rightPoint(qMax(scenePos.x(), m_startPoint.x()), qMax(scenePos.y(), m_startPoint.y()));

        m_tmpRect->setRect( QRectF(leftPoint,
                                  rightPoint));
    }*/
}

void GraphTool::select()
{
    auto gScene = qobject_cast<GraphicsScene*>(scene());
    auto graphSettings = dynamic_cast<GraphToolSettings*>(settings().get());

    if ( graphSettings == nullptr || gScene == nullptr ) return;

    auto bRect = graphSettings->boundingRect();

    m_xCursors = std::make_pair( std::make_shared<ConstraintLineItem>(QPointF(bRect.left(), scene()->height())),
                                 std::make_shared<ConstraintLineItem>(QPointF(bRect.right(), scene()->height())) );

    m_yCursors = std::make_pair(std::make_shared<ConstraintLineItem>(QPointF(scene()->width(), bRect.top()), Qt::Horizontal),
                                std::make_shared<ConstraintLineItem>(QPointF(scene()->width(), bRect.bottom()), Qt::Horizontal));


    m_xCursors.first->setZValue(gScene->currentLayerZ());
    m_xCursors.second->setZValue(gScene->currentLayerZ());

    m_yCursors.first->setZValue(gScene->currentLayerZ());
    m_yCursors.second->setZValue(gScene->currentLayerZ());

    scene()->addItem(m_xCursors.first.get());
    scene()->addItem(m_xCursors.second.get());

    scene()->addItem(m_yCursors.first.get());
    scene()->addItem(m_yCursors.second.get());


    auto result = connect(m_xCursors.first.get(),
                          &ConstraintLineItem::positionChanged,
                          this,
                          &GraphTool::on_cursorPositionChanged);
    Q_ASSERT(result);

    result = connect(m_xCursors.second.get(),
                     &ConstraintLineItem::positionChanged,
                     this,
                     &GraphTool::on_cursorPositionChanged);
    Q_ASSERT(result);

    result = connect(m_yCursors.first.get(),
                     &ConstraintLineItem::positionChanged,
                     this,
                     &GraphTool::on_cursorPositionChanged);
    Q_ASSERT(result);

    result = connect(m_yCursors.second.get(),
                     &ConstraintLineItem::positionChanged,
                     this,
                     &GraphTool::on_cursorPositionChanged);
    Q_ASSERT(result);

}

void GraphTool::unselect()
{
    removeFromScene();

    m_xCursors.first.reset();
    m_xCursors.second.reset();

    m_yCursors.first.reset();
    m_yCursors.second.reset();
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

    auto item = new QGraphicsPathItem(painterPath);
    item->setPen(pen);
    item->setZValue(gScene->currentLayerZ());

    gScene->addSceneCommand(new Commands::AddItemCommand(scene(),item));
}

void GraphTool::removeFromScene()
{
    if ( scene() ) {
        scene()->removeItem(m_xCursors.first.get());
        scene()->removeItem(m_xCursors.second.get());

        scene()->removeItem(m_yCursors.first.get());
        scene()->removeItem(m_yCursors.second.get());
    }
}
