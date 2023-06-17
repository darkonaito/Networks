#ifndef SERVER_HPP_DNW
#define SERVER_HPP_DNW

#include <unordered_map>
#include <list>
#include <tuple>
#include <mutex>
#include <string>

#include <boost/asio.hpp>

namespace dnw
{
    class Server
    {
        public:
            using Port   = boost::asio::ip::port_type;
            using Socket = boost::asio::ip::tcp::socket;

            using Processor = std::function<void(Socket&, Server&)>;

            void startListening();
            void startListening(const Port port);

            void stopListening();
            void stopListening(const Port port); 

            void setOnConnection(const Port port, const std::function<void(Socket&, Server&)>& processor);

            void applyOnClient(const std::function<void(Socket&)>& action);
            void applyOnClient(const Port port, const std::function<void(Socket&)>& action);
            void applyOnClient(Socket& socket, const std::function<void(Socket&)>& action);

            void closeConnection();
            void closeConnection(const Port port);
            void closeConnection(Socket& socket);

            size_t getClientCount()          const;
            size_t getClientCount(const Port port) const;

            //bool isOpen(Socket& socket);

        protected:
            std::unordered_map<
                Port,
                std::tuple<
                    Processor,
                    bool,
                    std::list<Socket>
                >
            > ports;

            boost::asio::io_context context;

            void listen(const Port port);

            Port getSocketPort(const Socket& socket);

            std::mutex mutex;
    };
}


#endif