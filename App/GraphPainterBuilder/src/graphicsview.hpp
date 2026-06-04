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

    void setCurrentLayerZ(int newCurrentLayerZ);
    void clearLayer(int layerZ);

    void setCurrentColor(const QColor &newCurrentColor);

public:
    void addSceneListener(ISceneListener* listener);
    void removeSceneListener(ISceneListener* listener);

    void addImage(const QString &path, QSize &imageSize);
    bool save(const QString &filePath);

    int currentLayerZ() const;

    QColor currentColor() const;

    void clear();

    bool isSceneEmpty() const;

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    std::shared_ptr<QUndoStack> m_undoStack; //!< Стек команд для отмены
    GraphicsScene *m_scene;         //!< Сцена
    QPoint m_origin;
};

#endif // GRAPHICSVIEW_HPP
