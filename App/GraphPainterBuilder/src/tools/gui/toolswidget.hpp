#ifndef TOOLSWIDGET_HPP
#define TOOLSWIDGET_HPP

#include "toollist.hpp"

#include <QPushButton>
#include <QDockWidget>

using TColorGetter = std::function<QColor()>;

/*!
 * \brief Dock Widget для отображения набора инструментов и их параметров
 */
class ToolsWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit ToolsWidget(const QString &title,
                         const TColorGetter &colorGetter,
                         const ToolList &toolList,
                         QWidget *parent = nullptr,
                         Qt::WindowFlags flags = Qt::WindowFlags());

Q_SIGNALS:
    void colorPickerClicked(const QColor &color);

public Q_SLOTS:
    /*!
     * \brief Обновить отображаемый цвет
     */
    void updateColor(const QColor &color);

private Q_SLOTS:
    void on_colorClicked();
    void on_currentToolChanged(ITool& tool);
    void on_toolDisabled();

private:
    QPushButton *m_colorButton;
    QLayout *m_settingsWidgetLayout;
    TColorGetter m_colorGetter;
};

#endif // TOOLSWIDGET_HPP
