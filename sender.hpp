#ifndef SENDER_HPP_DNW
#define SENDER_HPP_DNW

#include <boost/asio.hpp>
#include <string>
#include <string_view>

#include "client.hpp"

#include "buffer/buffer.hpp"
#include "misc/buffersocket.hpp"

namespace dnw
{
    class Sender;
}
 
class dnw::Sender
{
    public:
        Sender(const std::string_view address, const std::string_view port);

        void setAddress(const std::string_view address);
        void setPort   (const std::string_view port);

        Buffer receive(const Buffer& buffer);

        void send(const Buffer& buffer);

    protected:
        Client client;

        std::string address;
        std::string port;
};

#endif