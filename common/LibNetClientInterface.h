#pragma once
#include "CommonNetInclude.h"
#include "LibMsg.h"
#include "LibTSQueue.h"
#include "LibConnection.h"

namespace lsms {
    namespace libnet {

        template <typename T>
        class Client_Interface
        {
        public:

            Client_Interface(){}

            virtual ~Client_Interface()
            {
                disconnect();
            }


            bool connect(const std::string& host, const uint16_t port)
            {
                try 
                {
                    asio::ip::tcp::resolver resolver(m_context);
                    auto endpoints = resolver.resolve(host, std::to_string(port));


                    m_connection = std::make_unique<Connection<T>>(
                        Connection<T>::owner::client,
                        m_context,
                        asio::ip::tcp::socket(m_context), m_qMessagesIn 
                    );
                    m_connection->connect_to_server(endpoints);
                    
                    m_thrContext = std::thread([this] () { m_context.run(); });
                
                }
                catch (std::exception& e)
                {
                    //inform that client couldn't connect to ip adress
                    return false;
                }
                return true;
            }

            void disconnect()
            {
                if(is_connected())
                    m_connection->disconnect();

                m_context.stop();
                if(m_thrContext.joinable())
                    m_thrContext.join();
                
                m_connection.release();
            }

            bool is_connected()
            {
                if(m_connection)
                    return m_connection->is_connected();
                else    
                    return false;
            }

            TSQueue<libmsg::Owned_Message<T>>& getIncomming()
            {
                return m_qMessagesIn;
            }
        protected:
            asio::io_context m_context;
            std::thread m_thrContext

            std::unique_ptr<Connection<T>> m_connection;

        private:
            TSQueue<libmsg::Owned_Message<T>> m_qMessagesIn;
        };


    }
}