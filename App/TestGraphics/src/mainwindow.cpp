#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_sizeItem(nullptr)
    , m_state(NONE)
    , m_statusLabel(nullptr)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    m_statusLabel = new QLabel("", this);
    ui->statusbar->addWidget(m_statusLabel);

    m_view = new GraphicsView(this);
    m_view->setUpdatesEnabled(true);

    m_view->setSceneRect(QRectF(QPointF(0,0), QSize(1000, 1000)));
    centralWidget()->layout()->addWidget(m_view);

    auto result = QObject::connect(m_view,
                                   &GraphicsView::mouseMove,
                                   this,
                                   &MainWindow::onSceneMouseMove);
    Q_ASSERT(result);

    result = QObject::connect(m_view,
                              &GraphicsView::mousePressed,
                              this,
                              &MainWindow::onSceneMousePressed);
    Q_ASSERT(result);

    result = QObject::connect(m_view,
                              &GraphicsView::mouseReleased,
                              this,
                              &MainWindow::onSceneMouseReleased);
    Q_ASSERT(result);
}

void MainWindow::onSceneMouseMove(const QPointF &scenePos)
{
    if ( m_state == FIRST_RELEASED ) {
        m_sizeItem->setP2(scenePos);
        m_view->scene()->update();
    }

    m_statusLabel->setText(QString("x: %1, y: %2").arg(scenePos.x()).arg(scenePos.y()));
}

void MainWindow::onSceneMousePressed(Qt::MouseButton button, const QPointF &scenePos)
{
    if (button == Qt::LeftButton) {
        switch (m_state) {
        case NONE:
        {
            if ( !m_sizeItem ) {
                m_sizeItem = new SimpleSizeGraphicsItem();
                m_sizeItem->setHeight(-20);
                m_view->scene()->addItem(m_sizeItem);
            }

            m_sizeItem->setP1(scenePos);
            m_sizeItem->setP2(scenePos);

            m_state = FIRST_PRESSED;
            m_sizeItem->setVisible(true);
        }
            break;

        case FIRST_RELEASED:
            if ( m_sizeItem ) {
                m_sizeItem->setP2(scenePos);
                m_state = SECOND_PRESSED;
            } else {
                m_state = NONE;
            }

            break;

        case SECOND_PRESSED:
        case FIRST_PRESSED:
            break;

        }

    } else if (button == Qt::RightButton) {
        m_state = NONE;
        m_sizeItem->setVisible(false);
    }

    m_view->scene()->update();
}

void MainWindow::onSceneMouseReleased(Qt::MouseButton button, const QPointF &scenePos)
{
    if ( button == Qt::LeftButton ) {
        switch (m_state) {
        case FIRST_PRESSED:
            m_state = FIRST_RELEASED;
            break;
        case SECOND_PRESSED:
            m_state = NONE;
            break;
        case FIRST_RELEASED:
        case NONE:
            break;
        }
    }
}
