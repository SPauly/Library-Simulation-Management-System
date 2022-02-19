#pragma once
#include "CommonNetInclude.h"
#include "LibMsg.h"
#include "LibTSQueue.h"

namespace LSMS {
    namespace LIBNET {
        template <typename T>
        class Connection : public std::enable_shared_from_this<Connection<T>> //enables us to create a shared pointer to this -> similar to this just shared_ptr
        {
        public:
            Connection() 
            {};
            virtual ~Connection() {

            };

        public:
            bool connect_to_server();
            bool disconnect();
            bool is_connected() const;

        public:
            bool send(const LIBMSG::Message<T>& msg);

        protected:
            
        };
        

    }
}   