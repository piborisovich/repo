#include "layerswidget.hpp"

#include "strings.hpp"

#include <QPushButton>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>

LayersWidget::LayersWidget(const QString &title,
                           QWidget *parent,
                           Qt::WindowFlags flags) : QDockWidget(title, parent, flags)
    , m_layersList(new QListWidget)
    , m_nextLayerZ(40)

{
    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *layersLayout = new QVBoxLayout(contentWidget);

    layersLayout->setContentsMargins(5, 5, 5, 5);

    QListWidgetItem *layer3 = new QListWidgetItem(Strings::LAYER_NAME_3, m_layersList);
    layer3->setData(Qt::UserRole, 30);
    QListWidgetItem *layer2 = new QListWidgetItem(Strings::LAYER_NAME_2, m_layersList);
    layer2->setData(Qt::UserRole, 20);
    QListWidgetItem *layer1 = new QListWidgetItem(Strings::LAYER_NAME_1, m_layersList);
    layer1->setData(Qt::UserRole, 10);

    m_layersList->setCurrentRow(1);

    layersLayout->addWidget(m_layersList);

    QPushButton *addLayerButton = new QPushButton(Strings::ADD_LAYER_TEXT);
    addLayerButton->setStyleSheet("background-color: #f0fdf4; "
                                  "border: 1px solid #16a34a; "
                                  "min-height: 25px; "
                                  "font-weight: bold;");
    layersLayout->addWidget(addLayerButton);

    QPushButton *deleteLayerButton = new QPushButton(Strings::DELETE_LAYER_TEXT);
    deleteLayerButton->setStyleSheet("background-color: #fff5f5; "
                                     "border: 1px solid #e53e3e; "
                                     "min-height: 25px;");
    layersLayout->addWidget(deleteLayerButton);

    auto result = connect(m_layersList,
                          &QListWidget::currentRowChanged,
                          this,
                          &LayersWidget::on_currentRowChanged);
    Q_ASSERT(result);

    result = connect(addLayerButton,
                     &QPushButton::clicked,
                     this,
                     &LayersWidget::on_addButtonClicked);
    Q_ASSERT(result);

    result = connect(deleteLayerButton,
                     &QPushButton::clicked,
                     this,
                     &LayersWidget::on_removeButtonClicked);
    Q_ASSERT(result);

    setWidget(contentWidget);
}

void LayersWidget::on_currentRowChanged(int row)
{
    if (row < 0) return;
    QListWidgetItem *currentItem = m_layersList->item(row);
    if (currentItem) {
        emit layerZChanged(currentItem->data(Qt::UserRole).toInt());
    }
}

void LayersWidget::on_addButtonClicked(bool)
{
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
        emit layerZChanged(m_nextLayerZ);
        m_nextLayerZ += 10;
    }
}

void LayersWidget::on_removeButtonClicked(bool)
{
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
        emit layerRemoved(targetZ);
        delete currentItem;
    }
}
