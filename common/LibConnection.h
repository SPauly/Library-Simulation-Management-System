#pragma once
#include "CommonNetInclude.h"
#include "LibMsg.h"
#include "LibTSQueue.h"

namespace lsms
{
    namespace libnet
    {
        template <typename T>
        class Connection : public std::enable_shared_from_this<Connection<T>> // enables us to create a shared pointer to this -> similar to this just shared_ptr
        {
        public:

            enum class owner {
                server,
                client
            };

            Connection(owner parent, asio::io_context& asioContext, asio::ip::tcp::socket socket, TSQueue<Owned_Message<T>>& qIn)
                : m_asioContext(asioContext); m_socket(socket); m_qMessagesIn(qIn) 
            {
                m_nOwnerType = parent;
            }
            virtual ~Connection(){

            };

        public:
            bool connect_to_server();
            bool disconnect();
            bool is_connected() const;

        public:
            bool send(const libmsg::Message<T> &msg);

        protected:
            owner m_nOwnerType = owner::server;
            asio::ip::tcp::socket m_socket;

            asio::io_context &m_asioContext;

            TSQueue<libmsg::Message<T>> m_qMessageOut;
            TSQueue<libmsg::Owned_Message<T>> &m_qMessagesIn;
        };

    }
}