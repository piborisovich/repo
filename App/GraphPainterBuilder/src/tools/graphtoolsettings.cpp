#include "graphtoolsettings.hpp"

GraphToolSettings::GraphToolSettings() : ToolSettings()
    , m_pointsCount(100)
{
}

int GraphToolSettings::pointsCount() const
{
    return m_pointsCount;
}

void GraphToolSettings::setPointsCount(int newPointsCount)
{
    m_pointsCount = newPointsCount;
}
