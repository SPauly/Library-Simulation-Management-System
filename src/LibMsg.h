#pragma once
#include <cstdint>

namespace LIBMSG {

    enum class _UserType : uint8_t {
        _admin,
        _publisher,
        _user
    };

    enum class _Id : uint8_t {
        _id
    };

    enum class _MessageType : uint8_t {
        _request,
        _answer
        //add some more
    };

    struct _MessageBody {
        char* msg;

        _MessageBody(const char*);
        ~_MessageBody();
    };

    class LibMsg {
        const char* msg;
        _UserType m_ut;
        _Id m_id;
        _MessageType m_mt;
        _MessageBody m_msgb;
    public:
        
    };

}