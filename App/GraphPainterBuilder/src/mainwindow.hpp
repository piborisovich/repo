#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "core.hpp"
#include "iscenelistener.hpp"
#include "graphicsview.hpp"
#include "layerswidget.hpp"

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

private:
    void init();

    void mousePressed(Qt::MouseButton button, const QPointF &scenePos) override;
    void mouseReleased(Qt::MouseButton button, const QPointF &scenePos) override;
    void mouseMoved(const QPointF &scenePos) override;

    void updateColorButtonLayout(QColor color);

private:

    Ui::MainWindow *ui;
    GraphicsView *m_view;   //!< Представление
    QLabel *m_statusLabel;

    QDockWidget *m_toolDockWidget;   //!< Инструменты
    LayersWidget *m_layersWidget;    //!< Слои;

    QPushButton *m_colorButton;

    Core *m_core;

};
#endif // MAINWINDOW_HPP
