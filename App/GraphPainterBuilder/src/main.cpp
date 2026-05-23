#include "core.hpp"
#include"mainwindow.hpp"

int main(int argc, char *argv[])
{
    Core core(argc, argv);

    MainWindow w(&core);
    w.show();

    return core.exec();
}
