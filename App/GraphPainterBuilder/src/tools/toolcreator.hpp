#ifndef TOOLCREATOR_HPP
#define TOOLCREATOR_HPP

#include "itool.hpp"

class ToolCreator
{
public:

    enum class ToolTypes
    {
        SELECT_TOOL,
        DRAG_TOOL,
        BRUSH_TOOL,
        ERASE_TOOL,
        LINE_TOOL,
        RECTANGLE_TOOL,
        CIRCLE_TOOL,
        GRAPH_TOOL,
        PIPETTE_TOOL
    };


    static ITool *create(ToolTypes toolType);

public:



private:
    ToolCreator() = delete;
};

#endif // TOOLCREATOR_HPP
