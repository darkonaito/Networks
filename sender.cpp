#include "sender.hpp"

namespace dnw
{
    Sender::Sender(const std::string_view address, const std::string_view port)
    :
        address(address), port(port)
    {
    }

    void Sender::setAddress(const std::string_view address)
    {
        this->address = address;
    }

    void Sender::setPort(const std::string_view port)
    {
        this->port = port;
    }

    Buffer Sender::receive(const Buffer &buffer)
    {
        Buffer return_buffer;

        client.setOnConnection(
            [&](Client::Socket& socket, Client& client)
            {
                socket << buffer;
                socket >> return_buffer;
            }
        );

        client.connect(address, port);

        return return_buffer;
    }

    void Sender::send(const Buffer &buffer)
    {
        client.setOnConnection(
            [&](Client::Socket& socket, Client& client)
            {
                socket << buffer;
            }
        );

        client.connect(address, port);
    }    
}
