#ifndef IEPOINT_H
#define IEPOINT_H

#include <string>

class IEPoint
{
public:
    ~IEPoint() {}
    virtual std::string request(const std::string &data) = 0;
};

#endif // IEPOINT_H
