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
    , m_layersList(new QListWidget())
    , m_colorButton(new QPushButton())
    , m_nextLayerZ(40)
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

    // --- ЛЕВАЯ ПАНЕЛЬ: ИНСТРУМЕНТЫ И НАСТРОЙКИ ---
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);

    // Кнопки Назад / Вперед
    QHBoxLayout *undoRedoLayout = new QHBoxLayout();
    QPushButton *undoButton = new QPushButton(Strings::UNDO_TEXT);
    QPushButton *redoButton = new QPushButton(Strings::REDO_TEXT);
    undoRedoLayout->addWidget(undoButton);
    undoRedoLayout->addWidget(redoButton);
    leftLayout->addLayout(undoRedoLayout);

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

    // --- ПРАВАЯ ПАНЕЛЬ: УПРАВЛЕНИЕ СЛОЯМИ ---
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->addWidget(new QLabel(Strings::LAYERS_TITLE));

    QListWidgetItem *layer3 = new QListWidgetItem(Strings::LAYER_NAME_3, m_layersList);
    layer3->setData(Qt::UserRole, 30);
    QListWidgetItem *layer2 = new QListWidgetItem(Strings::LAYER_NAME_2, m_layersList);
    layer2->setData(Qt::UserRole, 20);
    QListWidgetItem *layer1 = new QListWidgetItem(Strings::LAYER_NAME_1, m_layersList);
    layer1->setData(Qt::UserRole, 10);

    m_layersList->setCurrentRow(1);
    m_view->setCurrentLayerZ(DEFAULT_LAYER_Z);
    m_view->setCurrentTool("Select");
    rightLayout->addWidget(m_layersList);

    QPushButton *addLayerButton = new QPushButton(Strings::ADD_LAYER_TEXT);
    addLayerButton->setStyleSheet("background-color: #f0fdf4; "
                                  "border: 1px solid #16a34a; "
                                  "min-height: 25px; "
                                  "font-weight: bold;");
    rightLayout->addWidget(addLayerButton);

    QPushButton *deleteLayerButton = new QPushButton(Strings::DELETE_LAYER_TEXT);
    deleteLayerButton->setStyleSheet("background-color: #fff5f5; "
                                     "border: 1px solid #e53e3e; "
                                     "min-height: 25px;");
    rightLayout->addWidget(deleteLayerButton);

    // --- ГЛАВНАЯ КОМПОНОВКА ---
    QWidget *centralWidget = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(leftPanel, 3);
    mainLayout->addWidget(m_view, 7);
    mainLayout->addWidget(rightPanel, 3);
    setCentralWidget(centralWidget);

    // --- ЛОГИКА, СИГНАЛЫ И СЛОТЫ ---

    // Привязка Undo/Redo к кнопкам интерфейса
    auto result = connect(undoButton,
                          &QPushButton::clicked,
                          m_view,
                          &GraphicsView::undo);
    Q_ASSERT(result);

    result = connect(redoButton,
                     &QPushButton::clicked,
                     m_view,
                     &GraphicsView::redo);
    Q_ASSERT(result);

    // Автоматическое управление доступностью кнопок Назад/Вперед
    undoButton->setEnabled(false);
    redoButton->setEnabled(false);

    result = connect(m_view,
                     &GraphicsView::canUndoChanged,
                     undoButton,
                     &QPushButton::setEnabled);
    Q_ASSERT(result);

    result = connect(m_view,
                     &GraphicsView::canRedoChanged,
                     redoButton,
                     &QPushButton::setEnabled);
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

    result = connect(m_layersList,
                     &QListWidget::currentRowChanged,
                     this,
                     [this](int row){
                         if (row < 0) return;
                         QListWidgetItem *currentItem = m_layersList->item(row);
                         if (currentItem) {
                             m_view->setCurrentLayerZ(currentItem->data(Qt::UserRole).toInt());
                         }
                     });
    Q_ASSERT(result);

    result = connect(addLayerButton,
                     &QPushButton::clicked,
                     this,
                     [this](){
                         bool ok;
                         QString layerName = QInputDialog::getText(this,
                                                                   Strings::NEW_LAYER_DIALOG_TITLE,
                                                                   Strings::NEW_LAYER_DIALOG_LABEL,
                                                                   QLineEdit::Normal,
                                                                   QString("Layer %1").arg(m_layersList->count() + 1),
                                                                   &ok);
                         if ( ok && !layerName.isEmpty() ) {
                             QListWidgetItem *newLayer = new QListWidgetItem(layerName);
                             newLayer->setData(Qt::UserRole, m_nextLayerZ);
                             m_layersList->insertItem(0, newLayer);
                             m_layersList->setCurrentItem(newLayer);
                             m_view->setCurrentLayerZ(m_nextLayerZ);
                             m_nextLayerZ += 10;
                         }
                     });
    Q_ASSERT(result);

    result = connect(deleteLayerButton,
                     &QPushButton::clicked,
                     this,
                     [this]() {
                         QListWidgetItem *currentItem = m_layersList->currentItem();
                         if (!currentItem) return;
                         if (m_layersList->count() <= 1) {
                             QMessageBox::warning(this,
                                                  Strings::REMOVE_LAYER_ERROR_MSG_BOX_TITLE,
                                                  Strings::REMOVE_LAYER_ERROR_MSG_BOX_MESSAGE);
                             return;
                         }


                         auto result = QMessageBox::question(this,
                                                             Strings::REMOVE_LAYER_MSG_BOX_TITLE,
                                                             QString(Strings::REMOVE_LAYER_MSG_BOX_QUESTION_TEMPLATE).arg(currentItem->text()),
                                                             QMessageBox::Yes | QMessageBox::No);
                         if (result == QMessageBox::Yes) {
                             int targetZ = currentItem->data(Qt::UserRole).toInt();
                             m_view->clearLayer(targetZ);
                             delete currentItem;
                         }
                     });
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
