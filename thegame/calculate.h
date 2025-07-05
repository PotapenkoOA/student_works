#ifndef CALCULATE_H
#define CALCULATE_H

#include <memory>
#include <iostream>

#include "field.h"
#include "context.h"

#include "icommand.h"

namespace Commands
{
std::string states[3] = {"white", "black", "busy"};

class calculate: public ICommand
{
    FieldPtr m_field;
    ContextPtr m_cntxt;

    unsigned char CheckUp( unsigned char count, int signX,  int signY)
    {
        Point p{m_cntxt->point.x, m_cntxt->point.y};
        for( int i = 1; i < 5; ++i )
        {
            p.y =  (unsigned char)(p.y + signY);
            p.x =  (unsigned char)(p.x + signX);
            unsigned char result = m_field->getPointStatus(p);

            if( result != m_cntxt->color )
                break;
            count++;
        }
        if( count == 4 )
            throw std::runtime_error("Игра окончена! " + states[m_cntxt->color] + " победил");

        return count;
    }

public:

    calculate( ContextPtr cntxt )
    {
        m_field = std::make_shared<Field>();
        m_cntxt = cntxt;
    }

    void execute()
    {
        if( m_field->setPointStatus( m_cntxt->color, m_cntxt->point ) )
        {
            //проверка наличия 5 в ряд
            unsigned char count = 0;
// проверка 4 вверх
            count = CheckUp( count, 0, -1);
            //std::cout<<"1:"<<(int)count<<std::endl;
// проверка 4 вниз
            count = CheckUp( count, 0, 1);
            //std::cout<<"2:"<<(int)count<<std::endl;

// проверка 4 влево
            count = CheckUp( 0, -1, 0);
            //std::cout<<"3:"<<(int)count<<std::endl;
// проверка 4 вправо
            count = CheckUp( count, 1, 0);
            //std::cout<<"4:"<<(int)count<<std::endl;

// проверка 1 диагонали слево сверху
            count = CheckUp( 0, -1, -1);
            //std::cout<<"5:"<<(int)count<<std::endl;
// проверка 1 диагонали справо снизу
            count = CheckUp( count, 1, 1);
            //std::cout<<"6:"<<(int)count<<std::endl;

// проверка 2 диагонали справо сверху
            count = CheckUp( 0, 1, -1);
            //std::cout<<"7:"<<(int)count<<std::endl;
// проверка 2 диагонали слево снизу
            count = CheckUp( count, -1, 1);
            //std::cout<<"8:"<<(int)count<<std::endl;
        }
    }
};

}
#endif // CALCULATE_H
