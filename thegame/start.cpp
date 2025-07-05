#include <iostream>
#include <stdexcept>
#include <boost/json.hpp>

#include "start.h"

namespace json = boost::json;

namespace Commands
{
Start::Start(IEPointPtr ep, ContextPtr ctx) : ICommand(), m_ep(ep), m_ctx(ctx) {}

void Start::execute()
{
    if(!m_ep)
        throw std::runtime_error("Wrong EP");

    json::object request;
    request["command"] = "start";

    auto answer = m_ep->request(json::serialize(request) + "\n");

    if(!m_ctx)
        throw std::runtime_error("Wrong Context");

    json::error_code ec;
    auto values = json::parse(answer, ec);

    if(ec.value() != 0)
    {
        std::cout << "Wrong JSON format: " << answer << std::endl;
        throw std::runtime_error("Wrong JSON format");
    }
    auto moveObject = values.as_object().at("move").as_object();
    m_ctx->point.x = moveObject.at("x").as_int64();
    m_ctx->point.y = moveObject.at("y").as_int64();
}


}
