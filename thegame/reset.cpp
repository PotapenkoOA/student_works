#include <iostream>
#include <stdexcept>
#include <boost/json.hpp>

#include "reset.h"

namespace json = boost::json;

namespace Commands
{
Reset::Reset(IEPointPtr ep) : ICommand(), m_ep(ep) {}

void Reset::execute()
{
    if(!m_ep)
        throw std::runtime_error("Wrong EP");

    json::object request;
    request["command"] = "reset";

    auto answer = m_ep->request(json::serialize(request) + "\n");

    json::error_code ec;
    auto values = json::parse(answer, ec);

    if(ec.value() != 0)
    {
        std::cout << "Wrong JSON format: " << answer << std::endl;
        throw std::runtime_error("Wrong JSON format");
    }
}

}
