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

    qreal penSize() const;
    void setPenSize(qreal newPenSize);

    QGraphicsView::DragMode dragMode() const;
    void setDragMode(const QGraphicsView::DragMode &newDragMode);

    virtual ~ToolSettings() {}

    qreal brushSize() const;
    void setBrushSize(qreal newBrushSize);

private:
    qreal m_penSize;
    qreal m_brushSize;
    QGraphicsView::DragMode m_dragMode;
};

#endif // TOOLSETTINGS_HPP
