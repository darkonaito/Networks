#include <fstream> 

template <>
inline std::ofstream& dnw::operator<<
        <std::ofstream>
    (
        std::ofstream& file,
        const dnw::Buffer& buff
    ) 
{
    const dnw::Buffer::Size buff_size {buff.getSize()};

    

    file.open(std::ios::binary | std::ios::out);

    

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
    using namespace boost;

    dnw::Buffer::Size buff_size;

    asio::read(sock, asio::buffer(&buff_size, sizeof(buff_size)));

    buff.resize(buff_size);

    asio::read(sock, asio::buffer(static_cast<void*>(buff.data.data()), buff_size));

    return sock;
}