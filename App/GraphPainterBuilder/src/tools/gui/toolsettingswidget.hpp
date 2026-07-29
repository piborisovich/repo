#ifndef TOOLSETTINGSWIDGET_HPP
#define TOOLSETTINGSWIDGET_HPP

#include "toolsettings.hpp"

#include <QLabel>
#include <QWidget>
#include <QFormLayout>

class ToolSettingsWidget : public QWidget
{
public:
    ToolSettingsWidget(std::shared_ptr<ToolSettings> settings,
                       QWidget* parent = nullptr,
                       Qt::WindowFlags f = Qt::WindowFlags());


protected:
    void addWidget(QWidget* widget);
    void addWidget(const QString &name, QWidget* widget);

protected:
    std::shared_ptr<ToolSettings> m_settings;

private Q_SLOTS:
    void on_penSizeChanged(int size);

private:

    QFormLayout *m_layout;
    QLabel *m_sizeLabel;
};

#endif // TOOLSETTINGSWIDGET_HPP
