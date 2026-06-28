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

    virtual ~ToolSettings() {}

private:
    int m_brushSize;
    QGraphicsView::DragMode m_dragMode;
};

#endif // TOOLSETTINGS_HPP
