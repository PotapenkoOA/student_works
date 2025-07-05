#ifndef IEPOINT_H
#define IEPOINT_H

#include <string>
#include <memory>

class IEPoint
{
public:
    ~IEPoint() {}
    virtual std::string request(const std::string &data) = 0;
};

using IEPointPtr = std::shared_ptr<IEPoint>;

#endif // IEPOINT_H
