#pragma once
#include <cstdint>

namespace LIBMSG {

    enum class _MessageType : uint8_t {
        _request,
        _answer
        //add some more
    };

    struct _Header {
        _MessageType mt{};
        uint32_t size = 0;
    };

    class LibMsg {
        _Header m_header{};
        
    public:
        
    };

}