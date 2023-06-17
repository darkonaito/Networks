#ifndef CLIENT_HPP_DNW
#define CLIENT_HPP_DNW

#include <boost/asio.hpp>

namespace dnw
{
    class Client
    {
        public:
            using Socket    = boost::asio::ip::tcp::socket;
            using Processor = std::function<void(Socket&, Client&)>;
            using Port      = boost::asio::ip::port_type;

            Client();

            void connect(std::string_view address, std::string_view port);
            void async_connect(std::string_view address, std::string_view port);

            void setOnConnection(const Processor& processor);

            void disconnect();

            ~Client();

        protected:
            boost::asio::io_context context;
            boost::asio::ip::tcp::resolver resolver;

            Socket socket;

            Processor processor; 
    };
}



#endif