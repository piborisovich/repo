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

    ~GraphTool() override;

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
    std::pair< std::shared_ptr<ConstraintLineItem>, std::shared_ptr<ConstraintLineItem> > m_xCursors;
    std::pair< std::shared_ptr<ConstraintLineItem>, std::shared_ptr<ConstraintLineItem> > m_yCursors;
};

#endif // GRAPHTOOL_HPP
