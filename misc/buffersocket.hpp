#ifndef BUFFERSOCKET_HPP_DNW
#define BUFFERSOCKET_HPP_DNW

#include <boost/asio.hpp> 
#include <mutex>

template <>
inline boost::asio::ip::tcp::socket& dnw::operator<<
        <boost::asio::ip::tcp::socket>
    (
        boost::asio::ip::tcp::socket& sock,
        const dnw::Buffer& buff
    ) 
{
    std::lock_guard<std::mutex> guard(buff.mutex);

    using namespace boost;

    const dnw::Buffer::Size buff_size {buff.getSize()};

    asio::write(sock, asio::buffer(&buff_size, sizeof(buff_size)));
    asio::write(sock, asio::buffer(static_cast<const void*>(buff.data.data()), buff_size));

    return sock;
}

template <>
inline boost::asio::ip::tcp::socket& dnw::operator>>
        <boost::asio::ip::tcp::socket>
    (
        boost::asio::ip::tcp::socket& sock,
        dnw::Buffer& buff
    )
{
    std::lock_guard<std::mutex> guard(buff.mutex);

    using namespace boost;

    dnw::Buffer::Size buff_size;

    asio::read(sock, asio::buffer(&buff_size, sizeof(buff_size)));

    buff.resize(buff_size);

    asio::read(sock, asio::buffer(static_cast<void*>(buff.data.data()), buff_size));

    return sock;
}

#endif