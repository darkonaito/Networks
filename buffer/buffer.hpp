
#ifndef DNW_BUFFER_HPP
#define DNW_BUFFER_HPP

#include <vector>
#include <string_view>

#include <mutex>

namespace dnw
{
    class Buffer
    {
        public:
            using Size = uint16_t;

            // Constructors
            Buffer() = default;
            Buffer(const Buffer& buffer);
            Buffer(Buffer&& buffer);
            //Buffer(const std::byte* data, Size size);

            /*
            template <typename T>
            Buffer(const T& source);
            */

            // Copy operators
            Buffer& operator=(const Buffer& buffer);
            Buffer& operator=(Buffer&& buffer);

            // Insertion
            Buffer& operator<<(const char* string);

            template <typename T>   
            friend Buffer& operator<<(Buffer& dest, const T& source);

            template <typename T>   
            friend Buffer& operator<<(Buffer&& dest, const T& source);            

            // Exctraction
            template <typename T>
            friend Buffer& operator>>(Buffer& dest, T& source);

            template <typename T>
            T retrieve();

            // Send
            template <typename T>
            friend T& operator<<(T& destination, const Buffer& buffer);

            // Receive
            template <typename T>
            friend T& operator>>(T& source, Buffer& buffer); 

            // Utilities
            Buffer& clearOnSend   (bool value);
            Buffer& clearOnReceive(bool value);

            Buffer& dropOnRetrieve(bool value);
        
            Buffer& clear();

            Size getSize() const;

            std::byte* getData();
            const std::byte* getData() const;

            void resize(Size size);

        protected:
            std::vector<std::byte> data;

            mutable std::mutex mutex;

            bool clear_on_send    {true};
            bool clear_on_receive {true};

            bool drop_on_retrieve {true};

            void cropBytes(size_t bytes);
    };
}

#include "buffer.ipp"

#endif
