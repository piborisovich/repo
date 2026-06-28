#ifndef LAYERSWIDGET_HPP
#define LAYERSWIDGET_HPP

#include <QDockWidget>
#include <QListWidget>

/*!
 * \brief Представление слоёв
 */
class LayersWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit LayersWidget(const QString &title,
                          QWidget *parent = nullptr,
                          Qt::WindowFlags flags = Qt::WindowFlags());

Q_SIGNALS:
    void layerZChanged(int layerZ);
    void layerRemoved(int layerZ);

private Q_SLOTS:
    void on_currentRowChanged(int row);
    void on_addButtonClicked(bool);
    void on_removeButtonClicked(bool);

private:
    QListWidget *m_layersList;
    int m_nextLayerZ;
};

#endif // LAYERSWIDGET_HPP
