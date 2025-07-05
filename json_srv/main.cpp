#include <boost/config.hpp>
#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/json.hpp>

using namespace std;

namespace asio = boost::asio;
using asio::ip::tcp;
namespace json = boost::json;
namespace opt = boost::program_options;

static volatile sig_atomic_t flag = 0;

 unsigned short port = 12345;

void signal_handler(int)
{
    flag = 1;
    std::cout << "Ctrl+C handled" << std::endl;
}

// Парсинг JSON из строки
json::value parseJson(const std::string& jsonStr) {
    json::error_code ec;
    json::value jv = json::parse(jsonStr, ec);
    if (ec) {
        throw std::runtime_error("JSON parse error: " + ec.message());
    }
    return jv;
}

// Обработка JSON-запроса и формирование ответа
std::string processRequest(const json::value& request) {
    json::object response;

    static int coordX = 15;
    static int coordY = 15;
    // Пример обработки (просто возвращаем полученные данные + статус)
    if (request.is_object())
    {
        if(request.as_object().contains("command"))
        {
            if(request.as_object().at("command").as_string() == "start")
            {
                json::object move;
                move["x"] = coordX;
                move["y"] = coordY;
                response["move"] = move;
                std::cout << json::serialize(response) << std::endl;
                return json::serialize(response);
            }
            else if(request.as_object().at("command").as_string() == "move")
            {
                json::object move;
                move["x"] = --coordX;
                move["y"] = --coordY;
                response["move"] = move;
                std::cout << json::serialize(response) << std::endl;
                return json::serialize(response);
            }
            else if(request.as_object().at("command").as_string() == "reset")
            {
                response["reply"] = "ok";
                coordX = 15;
                coordY = 15;
                std::cout << json::serialize(response) << std::endl;
                return json::serialize(response);
            }
        }
    }
    else
    {
        response["status"] = "error";
        response["message"] = "Invalid JSON format";
    }

    return json::serialize(response);
}

// Обработка подключения клиента
void handleClient(tcp::socket socket) {
    try {
        asio::streambuf buffer;
        asio::read_until(socket, buffer, '\n'); // Читаем до символа новой строки

        // Преобразуем буфер в строку
        std::istream stream(&buffer);
        std::string jsonStr;
        std::getline(stream, jsonStr);

        std::cout << port <<" Received JSON: " << jsonStr << std::endl;

        // Парсим JSON
        json::value request = parseJson(jsonStr);

        // Обрабатываем запрос и формируем ответ
        std::string response = processRequest(request) + "\n";

        // Отправляем ответ клиенту
        asio::write(socket, asio::buffer(response));

    } catch (const std::exception& e) {
        std::cerr << "Error in client handler: " << e.what() << std::endl;
    }namespace opt = boost::program_options;
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

    try {
        opt::options_description desc("Allowed options");
        desc.add_options()
            ("-p", opt::value<int>(), "Port number");
        opt::variables_map option_port;
        opt::store(opt::parse_command_line(argc, argv, desc), option_port);
        opt::notify(option_port);

        if(option_port.count("-p")) {
            port = option_port["-p"].as<int>();
        }
    } catch (...) {
        cout << "Programm options error" << endl;
    }

    try {
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

        std::cout << "Server started on port " << port << std::endl;

        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket); // Ожидаем подключения

            // Обрабатываем клиента в отдельном потоке
            std::thread(handleClient, std::move(socket)).detach();
        }

    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }

    return EXIT_SUCCESS;
}
