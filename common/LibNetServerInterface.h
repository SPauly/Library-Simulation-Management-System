#pragma once
#include "CommonNetInclude.h"
#include "LibMsg.h"
#include "LibTSQueue.h"
#include "LibConnection.h"

namespace lsms
{
    namespace libnet
    {
        template <typename T>
        class Server_Interface
        {
        public:
            Server_Interface(uint16_t port) : m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
            {
            }
            virtual ~Server_Interface()
            {
                stop_server();
            }

            bool start_server()
            {
                try
                {
                    wait_for_client_connection();
                    m_threadContext = std::thread([this]()
                                                  { m_asioContext.run(); });
                }
                catch (std::exception &e)
                {
                    // throw an error here
                }
                // inform that server is running here
            }
            void stop_server()
            {
                m_asioContext.stop();

                if (m_threadContext.joinable())
                    m_threadContext.join();

                // inform that server stopped
            }
            void wait_for_client_connection()
            {
                m_asioAcceptor.async_accept(
                    [this](std::error_code ec, asio::ip::tcp::socket socket)
                    {
                        if (!ec)
                        {
                            // display successfull connection

                            std::shared_ptr<Connection<T>> newconn = std::make_shared<Connection<T>>(Connection<T>::owner::server,
                                                                                                     m_asioContext, std::move(socket), m_qMessagesIn);

                            if (on_client_connect(newconn))
                            {
                                m_deqConnections.push_back(std::move(newconn));

                                m_deqConnections.back()->connect_to_client(nIDCounter++);

                                // inform server
                            }
                            else
                            {
                                // connection was denied
                            }
                        }
                        else
                        {
                            // inform of error
                        }

                        wait_for_client_connection();
                    });
            }
            void message_client(std::shared_ptr<Connection<T>> client, const Message<T> &msg)
            {
                if (client && client->is_connected())
                {
                    client->send(msg);
                }
                else
                {
                    on_client_disconnect(client);
                    client.reset();
                    m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
                }
            }
            void message_all_clients(const Message<T> &msg, std::shared_ptr<Connection<T>> pIgnoreClient = nullptr)
            {
                bool invalid_client_exists = false;

                for (auto &client : m_deqConnections)
                {
                    if (client && client->is_connected())
                    {
                        if (client != pIgnoreClient)
                            client->send(msg);
                    }
                    else
                    {
                        on_client_disconnect(client);
                        client.reset();
                        invalid_client_exists = true;
                    }
                }

                if (invalid_client_exists)
                {
                    m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr), m_deqConnections.end());
                }
            }

            void update(size_t nMaxMessages = -1)
            {
                size_t nMessageCount = 0;
                while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
                {
                    auto msg = m_qMessagesIn.pop_front();

                    on_message(msg.remote(), msg.msg);

                    nMessageCount++;
                }
            }

        protected:
            virtual bool on_client_connect(std::shared_ptr<Connection<T>> client)
            {
                return false;
            }

            virtual void on_client_disconnect(std::shared_ptr<Connection<T>> client)
            {
            }
            virtual void on_message(std::shared_ptr<Connection<T>> client, Message<T> &msg)
            {
            }

        protected:
            TSQueue<Owned_Message<T>> m_qMessagesIn;

            std::deque<std::shared_ptr<Connection<T>>> m_deqConnections;

            asio::io_context m_asioContext;
            std::thread m_threadContext;

            asio::ip::tcp::acceptor m_asioAcceptor;

            uint32_t nIDCounter = 10000;
        };

    }
}