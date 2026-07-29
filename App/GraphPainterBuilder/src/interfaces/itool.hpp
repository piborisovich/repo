#ifndef ITOOL_HPP
#define ITOOL_HPP

#include "toolsettings.hpp"
#include "graphicsscene.hpp"

#include <QPointF>
#include <QCursor>
#include <QPixmap>
#include <QToolButton>
#include <QWidget>

/*!
 * \brief Интерфейс инструмента
 */
class ITool : public QObject
{
    Q_OBJECT

public:

    explicit ITool(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~ITool() {}

Q_SIGNALS:
    void colorChangeRequested(const QColor &color);

public:
    virtual GraphicsScene *scene() = 0;
    virtual void setScene(GraphicsScene *scene) = 0;

    virtual QToolButton *button() = 0;

    virtual QWidget* settingsWidget() = 0;

    virtual std::shared_ptr<ToolSettings> settings() = 0;

    virtual const QCursor &getCursor() const = 0;

    virtual void handleMousePress(Qt::MouseButton button, const QPointF &scenePos) = 0;
    virtual void handleMouseRelease(Qt::MouseButton button, const QPointF &scenePos) = 0;
    virtual void handleMouseMove(Qt::MouseButtons buttons, const QPointF &scenePos) = 0;

    virtual void select() = 0;
    virtual void unselect() = 0;

    /*!
     * \brief Проверка нахождения вершины на сцене с учётом толщины кисти
     * \param scenePos
     * \return
     */
    virtual bool vertexOnScene(const QPointF &scenePos) const = 0;
};

#endif // ITOOL_HPP
