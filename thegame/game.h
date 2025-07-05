#ifndef GAME_H
#define GAME_H

#include <string>

#include "calculate.h"
#include "iepoint.h"
#include "icommand.h"

#include "start.h"
#include "move.h"
#include "reset.h"

namespace Commands
{
    class Game: public ICommand
    {
        IEPointPtr m_ep1;
        IEPointPtr m_ep2;

    public:
        Game(IEPointPtr ep1, IEPointPtr ep2)
        {
            m_ep1 = ep1;
            m_ep2 = ep2;
        }

        void execute()
        {
            auto ctx = std::make_shared<Context>();

            try {
                Start(m_ep1, ctx).execute();
                calculate cmd(ctx);
                PointPtr point = std::make_shared<Point>();
                point->x =  ctx->point.x;
                point->y =  ctx->point.y;

                Move cmd1(m_ep1, point, ctx );
                Move cmd2(m_ep2, point, ctx );

                while( 1 )
                {
                    ctx->color = 0;
                    cmd.execute();
                    cmd2.execute();
                    ctx->color = 1;
                    cmd.execute();
                    cmd1.execute();
                }
            }
            catch (std::exception &exc)
            {
                std::cout<<exc.what()<<std::endl<<std::endl;
                Reset(m_ep1).execute();
                Reset(m_ep2).execute();
            }
        }
    };
}
#endif // GAME_H
