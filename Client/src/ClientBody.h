#pragma once
#include <cstdint>

namespace LSMS {

    class Client {
    private:
        bool m_running = false;
        bool m_connected = false;

        bool mf_startup();
    public:
        void run_client();

    };
}