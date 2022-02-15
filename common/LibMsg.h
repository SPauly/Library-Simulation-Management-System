#pragma once
#include <cstdint>

namespace LIBMSG {

    enum class _MessageType : uint8_t {
        _request,
        _answer
        //add some more
    };

    template <typename T>
    struct MessageHeader {
        T mt{};
        uint32_t size = 0;
    };

    template <typename T>
    struct Message {
        MessageHeader<T> header{};
        std::vector<uint8_t> body;
    };

}