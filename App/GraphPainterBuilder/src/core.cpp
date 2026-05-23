#include "core.hpp"

#include "strings.hpp"

Core::Core(int &argc, char **argv)
    : QApplication(argc, argv)
{
    setApplicationName(Strings::APPLICATION_NAME);
}

Core::~Core()
{
}

void Core::on_ImageOpen()
{

}
