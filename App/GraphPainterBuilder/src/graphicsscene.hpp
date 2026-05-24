#ifndef GRAPHICSSCENE_HPP
#define GRAPHICSSCENE_HPP

#include "iscenelistener.hpp"

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QUndoStack>
#include <QList>

/*!
 * \brief Сцена
 */
class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene(QUndoStack *undoStack = nullptr, QObject *parent = nullptr);

    void addSceneListener(ISceneListener* listener);
    void removeSceneListener(ISceneListener* listener);

    int currentLayerZ() const;
    void setCurrentLayerZ(int newCurrentLayerZ);

    QString currentTool() const;
    void setCurrentTool(const QString &newCurrentTool);
    /*!
     * \brief addImage
     * \param path - Image path
     * \param imageSize - opened image size (return)
     */
    void addImage(const QString &path, QSize &imageSize);

    int brushSize() const;
    void setBrushSize(int newBrushSize);

    QColor currentColor() const;
    void setCurrentColor(const QColor &newCurrentColor);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void processDrawing(QPointF pos);

private:

    enum State {
        NONE,
        FIRST_PRESSED,
        FIRST_RELEASED,
        SECOND_PRESSED
    };

    static const QBrush BACKGROUND_BRUSH;

    int m_currentLayerZ;
    int m_brushSize;
    QColor m_currentColor;
    QString m_currentTool;
    QUndoStack *m_undoStack;


    QPointF m_startPoint;
    QGraphicsLineItem *m_previewLine = nullptr;
    QList<QGraphicsItem*> m_erasedItemsThisStroke; //!< Для группировки удаления ластиком

    QList<ISceneListener*> m_listeners;
};

#endif // GRAPHICSSCENE_HPP
