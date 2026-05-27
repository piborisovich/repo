#ifndef TOOLSWIDGET_HPP
#define TOOLSWIDGET_HPP

#include <QPushButton>
#include <QDockWidget>

using TColorGetter = std::function<QColor()>;

/*!
 * \brief Dock Widget для отображения набора инструментов их параметров
 */
class ToolsWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit ToolsWidget(const QString &title,
                         const TColorGetter &colorGetter,
                         QWidget *parent = nullptr,
                         Qt::WindowFlags flags = Qt::WindowFlags());

Q_SIGNALS:
    void currentToolChanged(const QString &tool);
    void brushSizeChanged(int size);
    void colorChanged(const QColor &color);

private Q_SLOTS:
    void on_colorClicked();

private:
    void updateColorButtonLayout(QColor color);

private:
    QPushButton *m_colorButton;
    TColorGetter m_colorGetter;
};

#endif // TOOLSWIDGET_HPP
