#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <ItemListView>

#include <QVBoxLayout>


using GUI::ItemListView;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initGUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initGUI()
{
    QStringList strings = { "Коротко", "Очень длинная строка с переносами слов", "Средний текст", "Многострочный\nтекст\nвручную"};
    QVBoxLayout *vLayout = new QVBoxLayout(ui->listViewBox);

    vLayout->addWidget( new ItemListView(strings, this) );


    auto result = QObject::connect(ui->actionExit,
                                   &QAction::triggered,
                                   this,
                                   &MainWindow::close);
    Q_ASSERT(result);
}
