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

    int brushSize() const;
    void setBrushSize(int newBrushSize);

    QGraphicsView::DragMode dragMode() const;
    void setDragMode(const QGraphicsView::DragMode &newDragMode);

    bool movable() const;
    void setMovable(bool newIsMovable);

    bool selectable() const;
    void setSelectable(bool newIsSelectable);

private:
    int m_brushSize;
    QGraphicsView::DragMode m_dragMode;
    bool m_isMovable;
    bool m_isSelectable;
};

#endif // TOOLSETTINGS_HPP
