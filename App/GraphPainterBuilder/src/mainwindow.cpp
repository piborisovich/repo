#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "strings.hpp"

#include <QFileDialog>
#include <QComboBox>
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>

static const int DEFAULT_LAYER_Z = 20;

MainWindow::MainWindow(Core *core, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_view(new GraphicsView(this))
    , m_statusLabel(new QLabel("", this))
    , m_toolsWidget(new ToolsWidget(Strings::TOOLS_TEXT, std::bind(&GraphicsView::currentColor, m_view ), this))
    , m_layersWidget(new LayersWidget(Strings::LAYERS_TITLE, this))
    , m_core(core)
{
    ui->setupUi(this);
    m_view->addSceneListener(this);

    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_imageImportTriggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    Strings::OPEN_IMAGE_DIALOIG_TITLE,
                                                    "",
                                                    "PNG Image (*.png);;JPEG Image (*.jpg)");
    if ( filePath.isEmpty() ) return;

    QSize imageSize;
    auto sr = m_view->sceneRect();

    m_view->addImage(filePath, imageSize);

    m_view->setSceneRect( QRectF( QPointF(0, 0),
                                QSizeF( qMax( sr.width(), qreal(imageSize.width()) ),
                                       qMax( sr.height(), qreal(imageSize.height()) ) ) ) );
}

void MainWindow::on_exportToImageTriggered()
{
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    Strings::SAVE_IMAGE_DIALOIG_TITLE,
                                                    "",
                                                    "PNG Image (*.png);;JPEG Image (*.jpg)");
    if ( filePath.isEmpty() ) return;

    if ( m_view->save(filePath) ) {
        QMessageBox::information(this,
                                 Strings::SAVE_SUCCESS_MSG_BOX_TITLE,
                                 Strings::SAVE_SUCCESS_MSG_BOX_MESSAGE);
    }
}

void MainWindow::on_clearCanvasTriggered()
{
    if ( m_view->isSceneEmpty() ) return;
    auto result = QMessageBox::question(this,
                                        Strings::CLEAR_CANVAS_MSG_BOX_TITLE,
                                        Strings::CLEAR_CANVAS_MSG_BOX_QUESTION,
                                        QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        m_view->clear();
    }
}

void MainWindow::on_currentToolChanged(const QString &tool)
{
    m_view->setCurrentTool(tool);

    if ( tool == "Select") {
        m_view->setDragMode(QGraphicsView::RubberBandDrag);
    } else if ( tool == "Drag") {
        m_view->setDragMode(QGraphicsView::ScrollHandDrag);
    } else {
        m_view->setDragMode(QGraphicsView::NoDrag);
    }
}

void MainWindow::init()
{
    setWindowTitle( Core::applicationName() );

    ui->statusbar->addWidget(m_statusLabel);

    centralWidget()->layout()->addWidget(m_view);

    addDockWidget(Qt::LeftDockWidgetArea, m_toolsWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_layersWidget);



    m_view->setCurrentLayerZ(DEFAULT_LAYER_Z);
    m_view->setCurrentTool("Select");


    // --- ГЛАВНАЯ КОМПОНОВКА ---
    QWidget *centralWidget = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(m_view, 7);
    setCentralWidget(centralWidget);

    // --- ЛОГИКА, СИГНАЛЫ И СЛОТЫ ---

    // Привязка Undo/Redo к кнопкам интерфейса
    auto result = connect(ui->actionUndo,
                          &QAction::triggered,
                          m_view,
                          &GraphicsView::undo);
    Q_ASSERT(result);

    result = connect(ui->actionRedo,
                     &QAction::triggered,
                     m_view,
                     &GraphicsView::redo);
    Q_ASSERT(result);

    // Автоматическое управление доступностью кнопок Назад/Вперед
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);

    result = connect(m_view,
                     &GraphicsView::canUndoChanged,
                     ui->actionUndo,
                     &QAction::setEnabled);
    Q_ASSERT(result);

    result = connect(m_view,
                     &GraphicsView::canRedoChanged,
                     ui->actionRedo,
                     &QAction::setEnabled);
    Q_ASSERT(result);

    result = connect(m_layersWidget,
                     &LayersWidget::layerZChanged,
                     m_view,
                     &GraphicsView::setCurrentLayerZ);
    Q_ASSERT(result);

    result = connect(m_layersWidget,
                     &LayersWidget::layerRemoved,
                     m_view,
                     &GraphicsView::clearLayer);
    Q_ASSERT(result);

    result = connect(ui->actionClear_canvas,
                     &QAction::triggered,
                     this,
                     &MainWindow::on_clearCanvasTriggered);
    Q_ASSERT(result);

    result = connect(ui->actionOpen,
                     &QAction::triggered,
                     this,
                     &MainWindow::on_imageImportTriggered);
    Q_ASSERT(result);

    result = connect(ui->actionExport,
                     &QAction::triggered,
                     this,
                     &MainWindow::on_exportToImageTriggered);
    Q_ASSERT(result);

    result = connect(ui->actionExit,
                          &QAction::triggered,
                          m_core,
                          &Core::exit);
    Q_ASSERT(result);

    result = connect(ui->actionLayer_panel,
                     &QAction::triggered,
                     m_layersWidget,
                     &LayersWidget::setVisible);
    Q_ASSERT(result);

    result = connect(ui->actionToolPanel,
                     &QAction::triggered,
                     m_toolsWidget,
                     &ToolsWidget::setVisible);
    Q_ASSERT(result);

    result = connect(m_layersWidget,
                     &LayersWidget::visibilityChanged,
                     ui->actionLayer_panel,
                     &QAction::setChecked);
    Q_ASSERT(result);

    result = connect(m_toolsWidget,
                     &ToolsWidget::visibilityChanged,
                     ui->actionToolPanel,
                     &QAction::setChecked);
    Q_ASSERT(result);

    result = connect(m_toolsWidget,
                     &ToolsWidget::currentToolChanged,
                     this,
                     &MainWindow::on_currentToolChanged);
    Q_ASSERT(result);

    result = connect(m_toolsWidget,
                     &ToolsWidget::brushSizeChanged,
                     m_view,
                     &GraphicsView::setBrushSize);
    Q_ASSERT(result);

    result = connect(m_toolsWidget,
                     &ToolsWidget::colorChanged,
                     m_view,
                     &GraphicsView::setCurrentColor);
    Q_ASSERT(result);

    ui->actionLayer_panel->setChecked(true);
}

void MainWindow::mousePressed(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(button);
    Q_UNUSED(scenePos);
}

void MainWindow::mouseReleased(Qt::MouseButton button, const QPointF &scenePos)
{
    Q_UNUSED(button);
    Q_UNUSED(scenePos);
}

void MainWindow::mouseMoved(const QPointF &scenePos)
{
    m_statusLabel->setText(QString("x: %1, y: %2").arg(scenePos.x()).arg(scenePos.y()));
}
