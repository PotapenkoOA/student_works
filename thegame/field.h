#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include <vector>
#include <memory.h>

#include "context.h"

class Field
{
    unsigned char m_size;
    std::vector<unsigned char> m_Field;
public:
    Field( )
    {
        //std::cout<<"***"<<std::endl;
        m_size = 31;
        m_Field = std::vector<unsigned char>(m_size*m_size, 2);
        //std::cout<<"***"<<std::endl;
    }

    bool setPointStatus( unsigned char status, Point point )
    {
        if( (point.x >= 31)  || (point.y >= 31))
            throw std::runtime_error("Точка за пределами поля");

        if( m_Field [point.x + m_size*point.y ] != 2)
            throw std::runtime_error("Поле уже занято");

        m_Field [point.x + m_size*point.y ] = status;
        return true;
    }

    unsigned char getPointStatus( Point point )
    {
        if( (point.x >= 31)  || (point.y >= 31))
            return 2;

        return m_Field [point.x + m_size*point.y ];
    }

/*    void Print()
    {
        std::cout<<":"<<(int)m_Field [13 + m_size*13 ]<<std::endl;
        std::cout<<":"<<(int)m_Field [14 + m_size*14 ]<<std::endl;
        std::cout<<":"<<(int)m_Field [15 + m_size*15 ]<<std::endl;
        std::cout<<":"<<(int)m_Field [16 + m_size*16 ]<<std::endl;
        std::cout<<":"<<(int)m_Field [17 + m_size*17 ]<<std::endl;
    }*/
};

using FieldPtr = std::shared_ptr<Field>;

#endif // FIELD_H
