#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <iostream>
#include <string>
#include <cstring>
#include <csignal>
#include <functional>
#include <list>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>
#include <dlfcn.h>
#include <filesystem>
#include <boost/program_options.hpp>
#include <boost/json.hpp>

#include "game.h"
#include "start.h"
#include "move.h"
#include "reset.h"
#include "players_list.h"
#include "start_player.h"

using namespace std;
namespace json = boost::json;

static volatile sig_atomic_t flag = 0;
static std::pair<std::string, std::string> authdata;

void signal_handler(int)
{
    flag = 1;
    std::cout << "Ctrl+C handled" << std::endl;
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);

    if(sigaction(SIGINT, &sa, 0) == -1)
    {
        std::cout << "Error install signal action" << std::endl;
        return EXIT_FAILURE;
    }

    /// запустить srvs
    //std::shared_ptr< std::list<std::string> > players =
    //    std::make_shared< std::list<std::string> >();
    //Commands::make_player_list("./players", players).execute();
    std::shared_ptr<pid_t> ppid1 = std::make_shared<pid_t>();
    std::shared_ptr<pid_t> ppid2 = std::make_shared<pid_t>();
    Commands::start_players("./players/players1/rendju-bot","-p54321", ppid1).execute();
    Commands::start_players("./players/players2/rendju-bot","-p54322", ppid2).execute();

    std::cout<<"\n";

   // sleep(1);
    /// епы
    auto ep1 = std::make_shared<EPoint>(54321);
    auto ep2 = std::make_shared<EPoint>(54322);

    /// запустить игру
    Commands::Game(ep1, ep2).execute();
    Commands::Game(ep2, ep1).execute();

    kill(*ppid1, SIGKILL);
    kill(*ppid2, SIGKILL);

    return EXIT_SUCCESS;
}
