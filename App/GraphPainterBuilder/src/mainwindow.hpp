#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "core.hpp"
#include "iscenelistener.hpp"
#include "graphicsview.hpp"
#include "layerswidget.hpp"
#include "toolswidget.hpp"
#include "propertiesdockwidget.hpp"

#include <QDateTime>
#include <QDockWidget>
#include <QListWidget>
#include <QPushButton>
#include <QUndoStack>

#define logDebug() qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz")



#include <QLabel>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow, protected ISceneListener
{
    Q_OBJECT

public:
    MainWindow(Core *core, QWidget *parent = nullptr);
    ~MainWindow();

private Q_SLOTS:
    void on_imageImportTriggered();      //!< Импорт
    void on_exportToImageTriggered();    //!< Экспорт
    void on_clearCanvasTriggered();      //!< Очистить сцену
    void on_blurEffectTriggered();       //!< Эффект размытия, открыть настройки
    void on_pixelationEffectTriggered(); //!< Эффект пикселизации

    void on_viewScaleChanged(qreal scale);     //!< Масштаб представления изменен
    void on_itemSelected(QGraphicsItem* item); //!< Графический элемент выделен
    void on_itemDeselected();                  //!< Снято выделение с графисекого элемента

    void on_showContextMenu(const QPoint &pos);

private:
    void init();

    void mousePressed(Qt::MouseButton button, const QPointF &scenePos) override;
    void mouseReleased(Qt::MouseButton button, const QPointF &scenePos) override;
    void mouseMoved(const QPointF &scenePos) override;

private:

    Ui::MainWindow *ui;
    Core *m_core;

    GraphicsView *m_view;   //!< Представление
    QLabel *m_xyLabel;
    QLabel *m_scaleLabel;

    ToolsWidget *m_toolsWidget;   //!< Инструменты
    LayersWidget *m_layersWidget;    //!< Слои;
    PropertiesDockWidget *m_propertiesWidget; //!< Свойства графического элемента
};
#endif // MAINWINDOW_HPP
