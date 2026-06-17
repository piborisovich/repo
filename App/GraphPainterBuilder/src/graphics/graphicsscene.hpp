#ifndef GRAPHICSSCENE_HPP
#define GRAPHICSSCENE_HPP

#include "iscenelistener.hpp"

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QUndoStack>
#include <QList>

class ITool;

/*!
 * \brief Сцена
 */
class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene(QObject *parent = nullptr);

Q_SIGNALS:
    void canUndoChanged(bool canUndo);
    void canRedoChanged(bool canRedo);
    void itemSelected(QGraphicsItem* item);
    void itemDeselected();

public Q_SLOTS:
    void undo();
    void redo();

public:
    void addSceneListener(ISceneListener* listener);
    void removeSceneListener(ISceneListener* listener);

    int currentLayerZ() const;
    void setCurrentLayerZ(int newCurrentLayerZ);

    void clearLayer(int layerZ);

    void changeCurrentTool(ITool *tool);
    /*!
     * \brief addImage
     * \param path - Image path
     * \param imageSize - opened image size (return)
     */
    void addImage(const QString &path, QSize &imageSize);

    QColor currentColor() const;
    void setCurrentColor(const QColor &newCurrentColor);

    /*!
     * \brief add Item to scene and create undo command
     * \param command
     */
    void addSceneCommand(QUndoCommand *command);

    void clearScene();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private Q_SLOTS:
    void on_selectionChanged();

private:

    enum State {
        NONE,
        FIRST_PRESSED,
        FIRST_RELEASED,
        SECOND_PRESSED
    };

    static const QBrush BACKGROUND_BRUSH;

    int m_currentLayerZ;

    QColor m_currentColor;

    ITool* m_currentTool;

    QUndoStack* m_undoStack;

    QList<ISceneListener*> m_listeners;
};

#endif // GRAPHICSSCENE_HPP
