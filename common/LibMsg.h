#pragma once
#include "CommonNetInclude.h"

namespace LSMS
{

    namespace LIBNET
    {

        namespace LIBMSG
        {

            enum class _MessageType : uint8_t
            {
                _request,
                _answer
                //add some more
            };

            template <typename T>
            struct MessageHeader
            {
                T id{};
                uint32_t size = 0;
            };

            template <typename T>
            struct Message
            {
                MessageHeader<T> header{};
                std::vector<uint8_t> body;

                size_t size() const
                {
                    return sizeof(MessageHeader<T>) + body.size();
                }

                friend std::ostream &operator<<(std::ostream &os, const Message<T> msg)
                {
                    os << "Message ID: " << (int)msg.header.id << " Message Size: " << msg.size;
                    return os;
                }

                template <typename DataType>
                friend Message<T> &operator<<(Message<T> &msg, const DataType &data)
                {
                    static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be handled by message object");

                    size_t current_body_size = msg.body.size();

                    msg.body.resize(current_body_size + sizeof(DataType));

                    std::memcpy(msg.body.data() + current_body_size, &data, sizeof(DataType));

                    msg.header.size = msg.size();

                    return msg;
                }

                template <typename DataType>
                friend Message<T> &operator>>(Message<T> &msg, const DataType &data)
                {
                    static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be handled by message object");

                    size_t cached_location = msg.body.size() - sizeof(DataType);

                    std::memcpy(&data, msg.body.data() + cached_location, sizeof(DataType));

                    msg.body.resize(cached_location);

                    msg.header.size = msg.size();

                    return msg;
                }
            };

        }
    }

}