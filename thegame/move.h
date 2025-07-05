#ifndef MOVE_H
#define MOVE_H

#include "icommand.h"
#include "context.h"
#include "epoint.h"

namespace Commands
{

class Move : public ICommand
{
    IEPointPtr m_ep;
    ContextPtr m_ctx;
    PointPtr m_point;

public:
    Move(IEPointPtr ep, PointPtr p, ContextPtr ctx);

    // ICommand interface
public:
    void execute();
};

}

#endif // MOVE_H
