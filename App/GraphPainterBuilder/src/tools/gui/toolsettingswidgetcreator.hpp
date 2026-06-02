#ifndef TOOLSETTINGSWIDGETCREATOR_HPP
#define TOOLSETTINGSWIDGETCREATOR_HPP

#include "toolsettings.hpp"

#include <QWidget>

class ToolSettingsWidgetCreator
{
public:
    static QWidget* create(std::shared_ptr<ToolSettings> settings);
};

#endif // TOOLSETTINGSWIDGETCREATOR_HPP
