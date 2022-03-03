#pragma once
#include <common/LibNet.h>  

namespace lsms {

    class Client : public libnet::Client_Interface<libnet::libmsg::_DefaultMessageType> {
    private:
        bool m_running = false;
        bool m_connected = false;

        bool mf_startup();
    public:
        void run_client();
        
    };

}