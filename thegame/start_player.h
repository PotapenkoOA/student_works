#ifndef START_PLAYER_H
#define START_PLAYER_H

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <thread>

#include <spawn.h>

namespace Commands
{

class start_players
{
    std::string m_name;
    std::string m_port;

    std::shared_ptr<pid_t> m_ppid;

    void launchWithPosixSpawn(const std::string& program, const std::vector<std::string>& args) {
        std::vector<char*> argv;
        argv.push_back(const_cast<char*>(program.c_str()));

        for (const auto& arg : args) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }
        argv.push_back(nullptr);



        pid_t pid;
        if (posix_spawn(&pid, program.c_str(), nullptr, nullptr, argv.data(), environ) != 0) {
            std::cerr << "posix_spawn() failed: " << strerror(errno) << std::endl;
            return;
        }

        *m_ppid = pid;

        std::cout << "Запущен процесс с PID: " << pid << std::endl;
    }

public:
    start_players( std::string name, std::string port, std::shared_ptr<pid_t> pid )
    {
        m_name = name;
        m_port = port;
        m_ppid = pid;
    }

    void execute()
    {
       // std::cout << "pa ba m" << std::endl;
        std::vector<std::string> args(1);
        args[0] = m_port;

       // std::cout<<"m_name:"<<(char*)m_name.c_str()<<" m_port:"<<(char*)m_port.c_str()<<std::endl;

        launchWithPosixSpawn(m_name, args);

    }
};
}

#endif // START_PLAYER_H
