#ifndef TOOLSETTINGS_HPP
#define TOOLSETTINGS_HPP

#include <QGraphicsView>

/*!
 * \brief Класс настроек инструмента
 */
class ToolSettings
{
public:

    ToolSettings();

    qreal brushSize() const;
    void setBrushSize(qreal newBrushSize);

    QGraphicsView::DragMode dragMode() const;
    void setDragMode(const QGraphicsView::DragMode &newDragMode);

    virtual ~ToolSettings() {}

private:
    qreal m_brushSize;
    QGraphicsView::DragMode m_dragMode;
};

#endif // TOOLSETTINGS_HPP
