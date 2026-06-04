#include "graphtoolsettings.hpp"

GraphToolSettings::GraphToolSettings() : ToolSettings() {}

size_t GraphToolSettings::pointCount() const
{
    return m_pointCount;
}

void GraphToolSettings::setPointCount(size_t newPointCount)
{
    m_pointCount = newPointCount;
}
