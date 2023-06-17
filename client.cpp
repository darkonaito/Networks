#include "client.hpp"

#include <thread>

dnw::Client::Client()
:
    socket(context),
    resolver(context)
{

}

void dnw::Client::connect(const std::string_view addr, const std::string_view port)
{
    if(socket.is_open())
    {
        return;
    }

    boost::asio::connect(socket, resolver.resolve(addr, port));

    processor(socket, *this);
}

void dnw::Client::async_connect(const std::string_view addr, const std::string_view port)
{
    if(socket.is_open())
    {
        return;
    }

    boost::asio::connect(socket, resolver.resolve(addr, port));

    std::thread thread {processor, std::ref(socket), std::ref(*this)};

    thread.detach();
}

void dnw::Client::setOnConnection(const Processor& proc)
{
    processor = proc;
}

void dnw::Client::disconnect()
{
    /*
    const std::array<std::byte, 1> message {static_cast<std::byte>(Messages::Closed)};

    boost::asio::write(socket, boost::asio::buffer(message));
    */

    socket.close();
}

dnw::Client::~Client()
{
    disconnect();
}