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
    , m_layersWidget(new LayersWidget(Strings::LAYERS_TITLE, this))
    , m_colorButton(new QPushButton())
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

void MainWindow::init()
{
    setWindowTitle( Core::applicationName() );

    ui->statusbar->addWidget(m_statusLabel);

    centralWidget()->layout()->addWidget(m_view);

    addDockWidget(Qt::RightDockWidgetArea, m_layersWidget);

    // --- ЛЕВАЯ ПАНЕЛЬ: ИНСТРУМЕНТЫ И НАСТРОЙКИ ---
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);

    // Выбор инструмента
    leftLayout->addWidget( new QLabel(Strings::TOOL_TEXT) );
    QComboBox *toolBox = new QComboBox();
    toolBox->addItems( {"Select", "Drag", "Brush", "Eraser", "Line", "Rectangle", "Circle"} );
    leftLayout->addWidget(toolBox);

    // Настройка размера
    leftLayout->addWidget(new QLabel(Strings::BRUSH_SIZE_TEXT));
    QSlider *sizeSlider = new QSlider(Qt::Horizontal);
    sizeSlider->setRange(1, 100);
    sizeSlider->setValue(5);
    leftLayout->addWidget(sizeSlider);

    // Выбор цвета
    leftLayout->addWidget(new QLabel(Strings::PALETTE_TEXT));
    updateColorButtonLayout(Qt::black);
    leftLayout->addWidget(m_colorButton);

    // Экспорт холста в файл
    leftLayout->addSpacing(15);

    QPushButton *importButton = new QPushButton(Strings::IMPORT_TEXT);
    importButton->setStyleSheet("background-color: #e0f2fe;"
                                "font-weight: bold;"
                                "min-height: 30px;"
                                "border: 1px solid #0284c7;");
    leftLayout->addWidget(importButton);

    QPushButton *exportButton = new QPushButton(Strings::EXPORT_TEXT);
    exportButton->setStyleSheet("background-color: #e0f2fe;"
                                "font-weight: bold;"
                                "min-height: 30px;"
                                "border: 1px solid #0284c7;");
    leftLayout->addWidget(exportButton);

    // Кнопка полной очистки холста
    leftLayout->addSpacing(10);
    QPushButton *clearCanvasButton = new QPushButton(Strings::CLEAR_CANVAS_TEXT);
    clearCanvasButton->setStyleSheet("background-color: #fee2e2;"
                                     "font-weight: bold;"
                                     "min-height: 30px;"
                                     "border: 1px solid #b91c1c;");
    leftLayout->addWidget(clearCanvasButton);

    leftLayout->addStretch();

    m_view->setCurrentLayerZ(DEFAULT_LAYER_Z);
    m_view->setCurrentTool("Select");


    // --- ГЛАВНАЯ КОМПОНОВКА ---
    QWidget *centralWidget = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(leftPanel, 3);
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

    result = connect(toolBox,
                     &QComboBox::currentTextChanged,
                     this,
                     [this](const QString &tool){
                         m_view->setCurrentTool(tool);

                         if ( tool == "Select") {
                             m_view->setDragMode(QGraphicsView::RubberBandDrag);
                         } else if ( tool == "Drag") {
                             m_view->setDragMode(QGraphicsView::ScrollHandDrag);
                         } else {
                             m_view->setDragMode(QGraphicsView::NoDrag);
                         }
    });
    Q_ASSERT(result);

    result = connect(sizeSlider,
                     &QSlider::valueChanged,
                     this,
                     [this](int value){
                         m_view->setBrushSize(value);
    });
    Q_ASSERT(result);

    result = connect(m_colorButton,
                     &QPushButton::clicked,
                     this,
                     [this](){
                         QColor color = QColorDialog::getColor(m_view->currentColor(),
                                                               this,
                                                               Strings::SELECT_COLOR_TEXT);
                         if (color.isValid()) {
                             m_view->setCurrentColor(color);
                             updateColorButtonLayout(color);
                         }
    });
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

    result = connect(clearCanvasButton,
                     &QPushButton::clicked,
                     this,
                     [this](){
                         if ( m_view->isSceneEmpty() ) return;
                         auto result = QMessageBox::question(this,
                                                             Strings::CLEAR_CANVAS_MSG_BOX_TITLE,
                                                             Strings::CLEAR_CANVAS_MSG_BOX_QUESTION,
                                                             QMessageBox::Yes | QMessageBox::No);
                         if (result == QMessageBox::Yes) {
                             m_view->clear();
                         }
                     });
    Q_ASSERT(result);

    result = connect(importButton,
                     &QPushButton::clicked,
                     this,
                     [this]() {
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

                     });
    Q_ASSERT(result);

    result = connect(exportButton,
                     &QPushButton::clicked,
                     this,
                     [this]() {
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
    });
    Q_ASSERT(result);

    result = connect(ui->actionExit,
                          &QAction::triggered,
                          m_core,
                          &Core::exit);
    Q_ASSERT(result);
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

void MainWindow::updateColorButtonLayout(QColor color)
{
    m_colorButton->setStyleSheet(QString("background-color: %1;"
                                         "min-height: 30px;"
                                         "border: 1px solid #555;").arg(color.name()));
}
