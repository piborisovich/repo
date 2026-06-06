#include "core.hpp"

#include "strings.hpp"
#include "toolcreator.hpp"

using ToolTypes = ToolCreator::ToolTypes;

ToolList Core::_tools = {};

Core::Core(int &argc, char **argv) : QApplication(argc, argv)
{
    setApplicationName(Strings::APPLICATION_NAME);

    _tools = {
        std::shared_ptr<ITool>( ToolCreator::create(ToolTypes::SELECT_TOOL) ),
        std::shared_ptr<ITool>( ToolCreator::create(ToolTypes::DRAG_TOOL) ),
        std::shared_ptr<ITool>( ToolCreator::create(ToolTypes::BRUSH_TOOL) ),
        std::shared_ptr<ITool>( ToolCreator::create(ToolTypes::ERASE_TOOL) ),
        std::shared_ptr<ITool>( ToolCreator::create(ToolTypes::LINE_TOOL) ),
        std::shared_ptr<ITool>( ToolCreator::create(ToolTypes::RECTANGLE_TOOL) ),
        std::shared_ptr<ITool>( ToolCreator::create(ToolTypes::CIRCLE_TOOL) ),
        std::shared_ptr<ITool>( ToolCreator::create(ToolTypes::GRAPH_TOOL) ),
        std::shared_ptr<ITool>( ToolCreator::create(ToolTypes::PIPETTE_TOOL) )
    };
}

Core::~Core()
{
}

const ToolList &Core::tools()
{
    return _tools;
}

void Core::changeSceneForTools(QGraphicsScene *scene)
{
    for ( auto &tool : _tools ) {
        if ( tool ) {
            tool->setScene(scene);
        }
    }
}
