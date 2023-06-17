#include "server.hpp"

#include <algorithm>
#include <thread>

#include <iostream>

void dnw::Server::startListening()
{
    for(auto& pair : ports)
    {
        startListening(pair.first);
    }
}

void dnw::Server::stopListening()
{
    for(auto& pair : ports)
    {
        stopListening(pair.first);
    }
}

void dnw::Server::startListening(const Port port)
{
    std::get<bool>(ports[port]) = true;

    std::thread thread {listen, this, port};

    thread.detach();
}

void dnw::Server::stopListening(const Port port)
{
    std::get<bool>(ports[port]) = false;
}

void dnw::Server::listen(const Port port)
{
    using namespace boost::asio::ip;

    tcp::acceptor acceptor {
        context,
        {tcp::v4(), port}
    };

    while(std::get<bool>(ports[port]))
    {
        Socket socket {context};

        acceptor.accept(socket);

        auto& sockets {std::get<std::list<Socket>>(ports[port])}; 

        sockets.push_back(std::move(socket));

        std::thread thread {std::get<Processor>(ports[port]), std::ref(sockets.back()), std::ref(*this)};
        
        thread.detach();
    }
}

void dnw::Server::setOnConnection(const Port port, const Processor& proc)
{
    std::get<Processor>(ports[port]) = proc;
}

void dnw::Server::closeConnection()
{
    for(auto& [key, tuple] : ports)
    {
        closeConnection(key);
    }
}

void dnw::Server::closeConnection(Port port)
{
    auto& sockets {std::get<std::list<Socket>>(ports[port])};

    for(auto& socket : sockets)
    {
        socket.close();
    }

    sockets.clear();
}

void dnw::Server::closeConnection(Socket& socket)
{
    using std::get;

    auto& sockets {get<std::list<Socket>>(ports[getSocketPort(socket)])};

    auto socket_iter {
        std::find_if(
            sockets.begin(),
            sockets.end(),
            [&](auto& s2)
            {
                return s2.local_endpoint() == socket.local_endpoint();
            } 
        )
    };

    if(socket_iter != sockets.end())
    {
        /*
        const std::array<std::byte, 1> message {static_cast<std::byte>(Messages::Closed)};
        
        boost::asio::write(socket, boost::asio::buffer(message));
        */
       
        socket.close();
        sockets.erase(socket_iter);
    }
}

size_t dnw::Server::getClientCount() const
{
    size_t total {};

    for(auto& pair : ports)
    {
        total += getClientCount(pair.first);
    }

    return total;
}

size_t dnw::Server::getClientCount(const Port port) const
{
    return std::get<std::list<Socket>>(ports.at(port)).size();
}

dnw::Server::Port dnw::Server::getSocketPort(const Socket& socket)
{
    return socket.local_endpoint().port();
}

void dnw::Server::applyOnClient(const std::function<void(Socket&)>& action)
{
    for(auto& [key, port] : ports)
    {
        applyOnClient(key, action);
    }
}

void dnw::Server::applyOnClient(const Port port, const std::function<void(Socket&)>& action)
{
    for(auto& socket : std::get<std::list<Socket>>(ports[port]))
    {
        applyOnClient(socket, action);
    }
}

void dnw::Server::applyOnClient(Socket& socket, const std::function<void(Socket&)>& action)
{
    std::lock_guard guard {mutex};

    action(socket);
}

/*
bool dnw::Server::isOpen(Socket& socket)
{
    
    const std::array<std::byte, 1> message {static_cast<std::byte>(Messages::ConnectionTest)};

    try
    {
        boost::asio::write(socket, boost::asio::buffer(message));
    }
    catch(const std::exception& e)
    {
        return false;
    }

    return true;
}
*/