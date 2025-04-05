#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <HexViewer>

#include <QVBoxLayout>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    m_tableView(new Hex::HexView(this))
{
    ui->setupUi(this);
    initGUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                QString("Открыть"),
                                                QString(),
                                                QString("Intel Hex (*.hex)"));

    if ( !path.isEmpty() ) {
        Hex::HexView *viewer = qobject_cast<Hex::HexView*>(m_tableView);
        viewer->load(path);
    }
}

void MainWindow::on_close()
{

}

void MainWindow::initGUI()
{
    QVBoxLayout *vLayout = new QVBoxLayout(centralWidget());
    vLayout->addWidget(m_tableView);

    auto result = QObject::connect(ui->exitAction,
                                   &QAction::triggered,
                                   this,
                                   &MainWindow::close);
    Q_ASSERT(result);

    result = QObject::connect(ui->openAction,
                              &QAction::triggered,
                              this,
                              &MainWindow::on_open);
    Q_ASSERT(result);

    result = QObject::connect(ui->closeAction,
                              &QAction::triggered,
                              this,
                              &MainWindow::on_close);
    Q_ASSERT(result);
}
