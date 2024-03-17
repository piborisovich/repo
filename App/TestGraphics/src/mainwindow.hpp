#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QDateTime>

#define logDebug() qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz")

#include "graphicsview.hpp"
#include "simplesizegraphicsitem.hpp"

#include <QLabel>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum State {
        NONE,
        FIRST_PRESSED,
        FIRST_RELEASED,
        SECOND_PRESSED
    };

private:
    void init();

private slots:
    void onSceneMouseMove(const QPointF &scenePos);
    void onSceneMousePressed(Qt::MouseButton button, const QPointF &scenePos);
    void onSceneMouseReleased(Qt::MouseButton button, const QPointF &scenePos);

private:

    Ui::MainWindow *ui;
    GraphicsView *m_view;
    SimpleSizeGraphicsItem *m_sizeItem;
    State m_state;

    QLabel *m_statusLabel;

};
#endif // MAINWINDOW_HPP
