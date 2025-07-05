#ifndef RESET_H
#define RESET_H

#include "icommand.h"
#include "context.h"
#include "epoint.h"

namespace Commands
{

class Reset : public ICommand
{
    IEPointPtr m_ep;

public:
    Reset(IEPointPtr ep);

    // ICommand interface
public:
    void execute();
};

}

#endif // RESET_H
