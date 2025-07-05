#ifndef EPOINT_H
#define EPOINT_H

#include "iepoint.h"

class EPoint : public IEPoint
{
    unsigned short m_port;

public:
    EPoint(unsigned short port);

    // IEPoint interface
public:
    std::string request(const std::string &data);
};


#endif // EPOINT_H
