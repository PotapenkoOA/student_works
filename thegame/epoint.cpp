#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <chrono>
#include <stdexcept>

#include "epoint.h"

using tcp =  boost::asio::ip::tcp;
namespace json = boost::json;

using boost::system::error_code;


EPoint::EPoint(unsigned short port) :
    m_port(port)
{}

bool read_with_timeout(tcp::socket& socket,
                      boost::asio::streambuf& buffer,
                      int timeout_milliseconds) {
    std::future<long unsigned int> read_result = std::async(std::launch::async, [&]() {
        return boost::asio::read_until(socket, buffer, '}');
    });

    if (read_result.wait_for(std::chrono::milliseconds(timeout_milliseconds)) == std::future_status::ready) {
        // std::cout << "sds: true" << std::endl;
        return true;  // Чтение завершено
    }

    // std::cout << "sds: false" << std::endl;
    socket.cancel();  // Прерываем операцию
    return false;     // Таймаут
}
std::string EPoint::request(const std::string &data)
{
    std::string response_string;
    try {
        // Создаем контекст ввода-вывода
        boost::asio::io_context io_context;

        // Создаем и соединяем сокет
        tcp::socket socket(io_context);

        bool connectionFlag = false;
        auto start = std::chrono::system_clock::now();
        while(!connectionFlag)
        {
            try
            {
                socket.connect(tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), m_port));
                connectionFlag = true;
            }
            catch(...)
            {
                if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() > 1000)
                {
                    throw std::runtime_error("connection timeout");
                }
            }
        }

        // Отправляем запрос
        boost::asio::write(socket, boost::asio::buffer(data));

        // Читаем ответ
        boost::asio::streambuf response;

        start = std::chrono::system_clock::now();
        while(read_with_timeout(socket, response, 100))
        {

            std::string recieved(boost::asio::buffer_cast<const char*>(response.data()), response.size());
            response_string += recieved;
            json::error_code ec;
            auto answer = json::parse(response_string, ec);

            if(ec.value() == 0)
            {
                std::cout << "Game.Recieved answer: " << answer << std::endl;
                return response_string;
            }
            if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() > 5000)
                throw std::runtime_error("query timeout");
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return "";
    }

    return "";
}
