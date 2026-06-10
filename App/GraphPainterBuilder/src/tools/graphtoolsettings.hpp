#ifndef GRAPHTOOLSETTINGS_HPP
#define GRAPHTOOLSETTINGS_HPP

#include "toolsettings.hpp"

class GraphToolSettings : public ToolSettings
{
public:
    GraphToolSettings();

    int pointsCount() const;
    void setPointsCount(int newPointsCount);

    QRectF boundingRect() const;
    void setBoundingRect(const QRectF &newBoundingRect);

private:
    int m_pointsCount;
    QRectF m_boundingRect;
};

#endif // GRAPHTOOLSETTINGS_HPP
