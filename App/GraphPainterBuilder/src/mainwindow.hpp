#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "core.hpp"
#include "iscenelistener.hpp"

#include <QDateTime>
#include <QListWidget>
#include <QPushButton>
#include <QUndoStack>

#define logDebug() qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz")

#include "graphicsscene.hpp"

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

private slots:
    void on_openTriggered();

private:
    void init();

    void mousePressed(Qt::MouseButton button, const QPointF &scenePos) override;
    void mouseReleased(Qt::MouseButton button, const QPointF &scenePos) override;
    void mouseMoved(const QPointF &scenePos) override;

    void updateColorButtonLayout(QColor color);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:

    Ui::MainWindow *ui;
    QUndoStack *m_undoStack;  //Стек команд для отмены
    GraphicsScene *m_scene; //Сцена
    QGraphicsView *m_view;  //Представление
    QLabel *m_statusLabel;

    QListWidget *m_layersList; //Слои
    QPushButton *m_colorButton;

    int m_nextLayerZ;

    Core *m_core;

};
#endif // MAINWINDOW_HPP
