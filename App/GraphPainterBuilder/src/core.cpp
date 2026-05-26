#include "core.hpp"

#include "strings.hpp"

ToolList Core::_tools = {};

Core::Core(int &argc, char **argv) : QApplication(argc, argv)
{
    setApplicationName(Strings::APPLICATION_NAME);
}

Core::~Core()
{
}

const ToolList &Core::tools()
{
    return _tools;
}
