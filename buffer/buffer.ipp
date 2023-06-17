#ifndef DNW_BUFFER_IPP
#define DNW_BUFFER_IPP

#include <string>
#include <iostream>
#include <cstring>
#include <thread>
#include <mutex>
#include "buffer.hpp"

// *** Enable specialization

template <typename T>
inline T& dnw::operator<<(T&, const dnw::Buffer&) = delete; 

template <typename T>
inline T& dnw::operator>>(T&, dnw::Buffer&) = delete;

// ***

template <typename T>
inline dnw::Buffer& dnw::operator<<(dnw::Buffer& buff, const T& source)
{
    const auto prev_size {buff.data.size()};

    buff.data.resize(prev_size + sizeof(source));

    memcpy(buff.data.data() + prev_size, &source, sizeof(T));

    return buff;
}

template <typename T>
inline dnw::Buffer &dnw::operator<<(Buffer &&dest, const T &source)
{
    return dest << source;
}

/*
template <>
inline dnw::Buffer& dnw::operator<<<dnw::Buffer>(dnw::Buffer& buff, const dnw::Buffer& source)
{
    const auto prev_size {buff.data.size()};

    buff.data.resize(buff.data.size() + source.getSize());

    memcpy(buff.data.data() + prev_size, source.getData(), source.getSize());

    return buff;
}
*/

inline dnw::Buffer& dnw::Buffer::operator<<(const char* const str)
{
    const auto prev_size {data.size()};

    data.resize(data.size() + strlen(str) + 1);

    memcpy(data.data() + prev_size, str, strlen(str) + 1);

    return *this;
}

template <>
inline dnw::Buffer& dnw::operator<< <std::string>(dnw::Buffer& buff, const std::string& str)
{
    const auto prev_size {buff.data.size()};

    buff.data.resize(buff.data.size() + str.length() + 1);

    memcpy(buff.data.data() + prev_size, str.data(), str.length() + 1);

    return buff;
}
/*
template <typename T>
inline dnw::Buffer::Buffer(const T& source)
{
    this->operator<< <T>(source);
}
*/

template <typename T>
inline T dnw::Buffer::retrieve()
{
    T obj;

    *this >> obj;

    return obj;
}


template <typename T>
inline dnw::Buffer& dnw::operator>>(dnw::Buffer& buff, T& dest)
{
    memcpy(&dest, buff.data.data(), sizeof(T));

    if(buff.drop_on_retrieve)
    {
        buff.cropBytes(sizeof(T));
    }

    return buff;
}

template <>
inline dnw::Buffer& dnw::operator>><std::string>(dnw::Buffer& buff, std::string& str) 
{
    // If buffer has bytes available, return string, else return ""

    if(buff.getSize())
    {
        str = std::string{static_cast<char*>(static_cast<void*>(buff.data.data()))};
    } else 
    {
        str = "";
    }
    
    if(buff.drop_on_retrieve)
    {
        buff.cropBytes(str.length() + 1);
    }

    return buff;
}

#endif