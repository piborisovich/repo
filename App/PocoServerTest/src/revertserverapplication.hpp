#ifndef REVERTSERVERAPPLICATION_HPP
#define REVERTSERVERAPPLICATION_HPP

#include <Poco/Util/ServerApplication.h>

using namespace Poco::Util;

class RevertServerApplication : public ServerApplication
{
public:
    RevertServerApplication();

protected:
    int main(const std::vector<std::string> &args) override;

private:
    static const Poco::UInt16 PORT = 28888;
};

#endif // REVERTSERVERAPPLICATION_HPP
