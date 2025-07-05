#ifndef PLAYERS_LIST_H
#define PLAYERS_LIST_H

#include <iostream>
#include <filesystem>
#include <list>
#include <memory>

#include "icommand.h"

namespace Commands
{

/// найти папку и запустить файлы
class make_player_list: public ICommand
{
    std::string m_path;
    std::shared_ptr< std::list<std::string> > m_players;

public:
    make_player_list( std::string path, std::shared_ptr< std::list<std::string> > players )
    {
        m_path = path;
        m_players = players;
    }

    void execute()
    {
        std::shared_ptr< std::list<std::string> > players =
            std::make_shared<std::list<std::string>>();
        for(const auto & entry : std::filesystem::directory_iterator(m_path))
        {
            std::string file_path = entry.path().string();
            players->push_back(file_path);

            //std::cout<<file_path<<std::endl;
        }

        int max = players->size();
        for( int i = 0; i < max; i += 2 )
        {
            std::string p1 = players->back();
            players->pop_back();
            std::string p2 = players->back();
            players->pop_back();

            m_players->push_back(p1);
            m_players->push_back(p2);

            m_players->push_back(p2);
            m_players->push_back(p1);
        }
    }
};

}

#endif // PLAYERS_LIST_H
