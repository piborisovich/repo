#ifndef GRAPHTOOLSETTINGSWIDGET_HPP
#define GRAPHTOOLSETTINGSWIDGET_HPP

#include "toolsettings.hpp"
#include "toolsettingswidget.hpp"

/*!
 * \brief Настройки для построения графика
 */
class GraphToolSettingsWidget : public ToolSettingsWidget
{
public:
    GraphToolSettingsWidget(std::shared_ptr<ToolSettings> settings,
                            QWidget* parent = nullptr,
                            Qt::WindowFlags f = Qt::WindowFlags());
};

#endif // GRAPHTOOLSETTINGSWIDGET_HPP
