#ifndef GRAPHTOOLSETTINGS_HPP
#define GRAPHTOOLSETTINGS_HPP

#include "toolsettings.hpp"

class GraphToolSettings : public ToolSettings
{
public:
    GraphToolSettings();

    size_t pointCount() const;
    void setPointCount(size_t newPointCount);

private:
    size_t m_pointCount;
};

#endif // GRAPHTOOLSETTINGS_HPP
