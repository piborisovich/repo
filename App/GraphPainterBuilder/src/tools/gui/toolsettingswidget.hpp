#ifndef TOOLSETTINGSWIDGET_HPP
#define TOOLSETTINGSWIDGET_HPP

#include "toolsettings.hpp"

#include <QWidget>

class ToolSettingsWidget : public QWidget
{
public:
    ToolSettingsWidget(std::shared_ptr<ToolSettings> settings,
                       QWidget* parent = nullptr,
                       Qt::WindowFlags f = Qt::WindowFlags());

private Q_SLOTS:
    void on_brushSizeChanged(int size);

private:
    std::shared_ptr<ToolSettings> m_settings;
};

#endif // TOOLSETTINGSWIDGET_HPP
