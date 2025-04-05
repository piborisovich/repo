#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QDateTime>
#include <QTableView>

#define logDebug() qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz")

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/*!
 * \brief Тест HexView
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_open();
    void on_close();

private:
    void initGUI();

private:
    Ui::MainWindow *ui;
    QTableView *m_tableView;
};
#endif // MAINWINDOW_HPP
