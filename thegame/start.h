#ifndef START_H
#define START_H

#include "icommand.h"
#include "context.h"
#include "epoint.h"

namespace Commands
{

class Start : public ICommand
{
    IEPointPtr m_ep;
    ContextPtr m_ctx;

public:
    Start(IEPointPtr ep, ContextPtr ctx);

    // ICommand interface
public:
    void execute();
};

}

#endif // START_H
