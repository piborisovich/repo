#include "toolsettingswidgetcreator.hpp"

#include "toolsettingswidget.hpp"

#include "graphtoolsettings.hpp"
#include "graphtoolsettingswidget.hpp"

QWidget *ToolSettingsWidgetCreator::create(std::shared_ptr<ToolSettings> settings)
{
    GraphToolSettings *gSettings = dynamic_cast<GraphToolSettings*>( settings.get() );

    if ( gSettings ) {
        return new GraphToolSettingsWidget(settings);
    }

    return new ToolSettingsWidget(settings);
}
