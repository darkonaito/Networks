#include "buffer.hpp"

dnw::Buffer::Buffer(const dnw::Buffer& buff)
{
    this->operator=(buff);
}

dnw::Buffer::Buffer(dnw::Buffer&& buff)
{
    this->operator=(std::move(buff));
}

/*
dnw::Buffer::Buffer(const std::byte* t_data, Size size)
{
    data.resize(size);

    memcpy(data.data(), t_data, size);
}
*/

dnw::Buffer& dnw::Buffer::operator=(const dnw::Buffer& buff)
{
    data = buff.data;

    return *this;
}

dnw::Buffer& dnw::Buffer::operator=(dnw::Buffer&& t_buff)
{
    data = std::move(t_buff.data);

    return *this;
}

dnw::Buffer& dnw::Buffer::clearOnSend(const bool val)
{
    clear_on_send = val;

    return *this;
}

dnw::Buffer& dnw::Buffer::clearOnReceive(const bool val)
{
    clear_on_receive = val;

    return *this;
}

dnw::Buffer& dnw::Buffer::dropOnRetrieve(const bool val)
{
    drop_on_retrieve = val;

    return *this;
}

dnw::Buffer& dnw::Buffer::clear()
{
    data.clear();

    return *this;
}

dnw::Buffer::Size dnw::Buffer::getSize() const 
{
    return data.size();
}

void dnw::Buffer::cropBytes(const size_t bytes)
{
    data.erase(data.begin(), data.begin() + bytes);
}

std::byte* dnw::Buffer::getData()
{
    return data.data();
}

const std::byte* dnw::Buffer::getData() const
{
    return data.data();
}


void dnw::Buffer::resize(const Size size)
{
    data.resize(size);
}