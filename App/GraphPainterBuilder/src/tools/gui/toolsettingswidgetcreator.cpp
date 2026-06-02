#include "toolsettingswidgetcreator.hpp"

#include "toolsettingswidget.hpp"

QWidget *ToolSettingsWidgetCreator::create(std::shared_ptr<ToolSettings> settings)
{
    return new ToolSettingsWidget(settings);
}
