#ifndef GRAPHICSVIEW_HPP
#define GRAPHICSVIEW_HPP

#include "graphicsscene.hpp"

#include <QUndoStack>
#include <QGraphicsView>

/*!
 * \brief Отображение рабочего пространства
 */
class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = nullptr);

Q_SIGNALS:
    void canUndoChanged(bool canUndo);
    void canRedoChanged(bool canRedo);

public Q_SLOTS:
    void undo();
    void redo();

public:
    void addSceneListener(ISceneListener* listener);
    void removeSceneListener(ISceneListener* listener);

    void addImage(const QString &path, QSize &imageSize);
    bool save(const QString &filePath);

    int currentLayerZ() const;
    void setCurrentLayerZ(int newCurrentLayerZ);

    QString currentTool() const;
    void setCurrentTool(const QString &newCurrentTool);

    int brushSize() const;
    void setBrushSize(int newBrushSize);

    QColor currentColor() const;
    void setCurrentColor(const QColor &newCurrentColor);

    void clearLayer(int layerZ);
    void clear();

    bool isSceneEmpty() const;

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    QUndoStack *m_undoStack; //!< Стек команд для отмены
    GraphicsScene *m_scene;  //!< Сцена
};

#endif // GRAPHICSVIEW_HPP
