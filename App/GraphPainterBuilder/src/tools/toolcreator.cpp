#include "toolcreator.hpp"

#include "selecttool.hpp"
#include "dragtool.hpp"
#include "brushtool.hpp"
#include "erasertool.hpp"
#include "linetool.hpp"
#include "rectangletool.hpp"
#include "circletool.hpp"
#include "graphtool.hpp"


ITool *ToolCreator::create(ToolTypes toolType)
{
    switch(toolType) {
    case ToolTypes::SELECT_TOOL: return new SelectTool();
    case ToolTypes::DRAG_TOOL: return new DragTool();
    case ToolTypes::BRUSH_TOOL: return new BrushTool();
    case ToolTypes::ERASE_TOOL: return new EraserTool();
    case ToolTypes::LINE_TOOL: return new LineTool();
    case ToolTypes::RECTANGLE_TOOL: return new RectangleTool();
    case ToolTypes::CIRCLE_TOOL: return new CircleTool();
    case ToolTypes::GRAPH_TOOL: return new GraphTool();
        default:;
    }
    return nullptr;
}
