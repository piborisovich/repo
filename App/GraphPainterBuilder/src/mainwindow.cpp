#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "strings.hpp"

#include <QFileDialog>
#include <QComboBox>
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>

static const int DEFAULT_LAYER_Z = 20;

static const char* SCALE_TAMPLATE = "%1 %";

MainWindow::MainWindow(Core *core, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_core(core)
    , m_view( new GraphicsView(this) )
    , m_xyLabel( new QLabel("", this))
    , m_scaleLabel( new QLabel(QString(SCALE_TAMPLATE).arg(100), this))
    , m_toolsWidget( new ToolsWidget(Strings::TOOLS_TEXT, std::bind(&GraphicsView::currentColor, m_view ), core->tools(), this) )
    , m_layersWidget( new LayersWidget(Strings::LAYERS_TITLE, this ) )
    , m_propertiesWidget(nullptr)
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

void MainWindow::on_viewScaleChanged(qreal scale)
{
    m_scaleLabel->setText( QString(SCALE_TAMPLATE).arg( int(scale * 100) ) );
}

void MainWindow::on_itemSelected(QGraphicsItem *item)
{
    if ( m_propertiesWidget ) {
        removeDockWidget(m_propertiesWidget);
        m_propertiesWidget->deleteLater();
        m_propertiesWidget = nullptr;
    }

    if ( item ) {
        m_propertiesWidget = new PropertiesDockWidget(Strings::PROPERTIES_WIDGET_TITLE,
                                                      item,
                                                      this);
        addDockWidget(Qt::RightDockWidgetArea, m_propertiesWidget);
    }
}

void MainWindow::on_itemDeselected()
{
    if ( m_propertiesWidget ) {
        removeDockWidget(m_propertiesWidget);
        m_propertiesWidget->deleteLater();
        m_propertiesWidget = nullptr;
    }
}

void MainWindow::init()
{
    setWindowTitle( Core::applicationName() );

    ui->statusbar->addWidget(m_scaleLabel);
    ui->statusbar->addWidget(m_xyLabel);

    centralWidget()->layout()->addWidget(m_view);

    addDockWidget(Qt::LeftDockWidgetArea, m_toolsWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_layersWidget);


    m_view->setCurrentLayerZ(DEFAULT_LAYER_Z);
   // m_view->setCurrentTool("Select");


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

    result = connect(m_view,
                     &GraphicsView::scaleChanged,
                     this,
                     &MainWindow::on_viewScaleChanged);
    Q_ASSERT(result);

    result = connect(m_view,
                     &GraphicsView::itemSelected,
                     this,
                     &MainWindow::on_itemSelected);
    Q_ASSERT(result);

    result = connect(m_view,
                     &GraphicsView::itemDeselected,
                     this,
                     &MainWindow::on_itemDeselected);
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
                     &ToolsWidget::colorPickerClicked,
                     m_view,
                     &GraphicsView::setCurrentrColor);
    Q_ASSERT(result);

    for ( auto &tool : m_core->tools() ) {
        result = connect(tool.get(),
                         &ITool::colorChangeRequested,
                         m_toolsWidget,
                         &ToolsWidget::updateColor);
        Q_ASSERT(result);

        result = connect(tool.get(),
                         &ITool::colorChangeRequested,
                         m_view,
                         &GraphicsView::setCurrentrColor);
        Q_ASSERT(result);
    }

    ui->actionLayer_panel->setChecked(true);

    m_core->changeSceneForTools( m_view->scene() );
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
    m_xyLabel->setText(QString("x: %1, y: %2").arg(scenePos.x()).arg(scenePos.y()));
}
