#ifndef GRAPHTOOL_HPP
#define GRAPHTOOL_HPP

#include "tool.hpp"
#include "constraintlineitem.hpp"

/*!
 * \brief Работа с графиком
 */
class GraphTool : public Tool
{
    Q_OBJECT
public:
    explicit GraphTool(QObject* parent = nullptr);

    void handleMousePress(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos) override;
    void handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos) override;

protected:
    void select() override;
    void unselect() override;

private Q_SLOTS:
    void on_cursorPositionChanged(const QPointF &point);
    void on_startPlot();
private:
    void removeFromScene();

private:
    std::pair< ConstraintLineItem*, ConstraintLineItem* > m_xCursors;
    std::pair< ConstraintLineItem*, ConstraintLineItem* > m_yCursors;
};

#endif // GRAPHTOOL_HPP
