#ifndef GRAPHICSVIEW_HPP
#define GRAPHICSVIEW_HPP

#include "graphicsscene.hpp"

#include <QGraphicsView>

/*!
 * \brief Отображение рабочего пространства
 */
class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = nullptr);

    ~GraphicsView();

Q_SIGNALS:
    void canUndoChanged(bool canUndo);
    void canRedoChanged(bool canRedo);
    void scaleChanged(qreal scale);
    void itemSelected(QGraphicsItem *item);
    void itemDeselected();

public Q_SLOTS:
    void undo();
    void redo();

    void setCurrentLayerZ(int newCurrentLayerZ);
    void setCurrentrColor(const QColor &color);
    void clearLayer(int layerZ);

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
    GraphicsScene *m_scene;         //!< Сцена
    QPoint m_origin;
    qreal m_scale;
};

#endif // GRAPHICSVIEW_HPP
