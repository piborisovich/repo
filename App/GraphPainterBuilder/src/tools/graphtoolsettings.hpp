#ifndef GRAPHTOOLSETTINGS_HPP
#define GRAPHTOOLSETTINGS_HPP

#include "toolsettings.hpp"

class GraphToolSettings : public ToolSettings
{
public:
    GraphToolSettings();

    int pointsCount() const;
    void setPointsCount(int newPointsCount);

private:
    int m_pointsCount;
};

#endif // GRAPHTOOLSETTINGS_HPP
