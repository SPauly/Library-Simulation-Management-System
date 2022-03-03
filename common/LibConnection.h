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
            enum class owner
            {
                server,
                client
            };

            Connection(owner parent, asio::io_context &asioContext, asio::ip::tcp::socket socket, TSQueue<Owned_Message<T>> &qIn)
                : m_asioContext(asioContext);
            m_socket(socket);
            m_qMessagesIn(qIn)
            {
                m_nOwnerType = parent;
            }
            virtual ~Connection(){

            };

        public:
            bool connect_to_server(const asio::ip::tcp::resolver::results_type& endpoints)
            {
                if(m_nOwnerType == owner::client)
                {
                    asio::async_connect(m_socket, endpoints, 
                        [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
                        {
                            if(!ec)
                            {
                                mf_read_header();
                            }
                        });
                }
            }
            void connect_to_client(uint32_t uid = 0)
            {
                if (m_nOwnerType == owner::server)
                {
                    if (m_socket.is_open())
                    {
                        id = uid;
                        mf_read_header();
                    }
                }
            }

            void disconnect()
            {
                if (is_connected())
                    asio::post(m_asioContext, [this]()
                               { m_socket.close(); });
            }
            bool is_connected() const
            {
                return m_socket.is_open();
            }
            uint32_t get_id() const
            {
                return m_id;
            }

        public:
            bool send(const libmsg::Message<T> &msg)
            {
                asio::post(m_asioContext,
                           [this, msg]()
                           {
                               bool b_is_writing_messages = !m_qMessagesOut.empty();
                               m_qMessagesOut.push_back(msg);
                               if (!b_is_writing_messages)
                               {
                                   mf_write_header();
                               }
                           });
            }

        private:
            void mf_read_header()
            {
                asio::async_read(m_socket, asio::buffer(&m_TempMsgIn.header, sizeof(libmsg::MessageHeader<T>)),
                                 [this](std::error_code ec, std::size_t length)
                                 {
                                     if (!ec)
                                     {
                                         if (m_TempMsgIn.header.size > 0)
                                         {
                                             m_TempMsgIn.body.resize(m_TempMsgIn.header.size);
                                             mf_read_body();
                                         }
                                         else
                                         {
                                             mf_add_to_incoming_message_queue();
                                         }
                                     }
                                     else
                                     {
                                         // inform server here
                                         m_socket.close();
                                     }
                                 });
            }

            void mf_read_body()
            {
                asio::async_read(m_socket, asio::buffer(m_TempMsgIn.body, m_TempMsgIn.body.size()),
                                 [this](std::error_code ec, std::size_t length)
                                 {
                                     if (!ec)
                                     {
                                         mf_add_to_incoming_message_queue();
                                     }
                                     else
                                     {
                                         // failed to read and add to body
                                         m_socket.close();
                                     }
                                 });
            }

            void mf_write_header()
            {
                asio::async_write(m_socket, asio::buffer(&m_qMessagesOut.front().header, sizeof(libmsg::MessageHeader<T>)),
                                  [this](std::error_code ec, std::size_t lenght)
                                  {
                                      if (!ec)
                                      {
                                          if (m_qMessagesOut.front().body.size() > 0)
                                          {
                                              mf_write_body();
                                          }
                                          else
                                          {
                                              m_qMessagesOut.pop_front();
                                              if (!m_qMessagesOut.empty())
                                              {
                                                  mf_write_header();
                                              }
                                          }
                                      }
                                      else
                                      {
                                          // inform that something went wrong
                                          m_socket.close();
                                      }
                                  });
            }

            void mf_write_body()
            {
                asio::async_write(m_socket, asio::buffer(&m_qMessagesOut.front().body.data(), m_qMessagesOut.front().body.size()),
                                  [this](std::error_code, std::size_t length)
                                  {
                                      if (!ec)
                                      {
                                          m_qMessagesOut.pop_front();

                                          if (!m_qMessagesOut.empty())
                                          {
                                              mf_write_header();
                                          }
                                      }
                                      else
                                      {
                                          // inform that sending failed
                                          m_socket.close();
                                      }
                                  });
            }
            void mf_add_to_incoming_message_queue()
            {
                if (m_nOwnerType == owner::server)
                    m_qMessagesIn.push_back({this->shared_from_this(), m_TempMsgIn});
                else
                    m_qMessagesIn.push_back({nullptr, m_TempMsgIn});

                mf_read_header();
            }

        protected:
            asio::ip::tcp::socket m_socket;

            asio::io_context &m_asioContext;

            TSQueue<libmsg::Message<T>> m_qMessagesOut;
            TSQueue<libmsg::Owned_Message<T>> &m_qMessagesIn;
            libmsg::Message<T> m_TempMsgIn;

            owner m_nOwnerType = owner::server;
            uint32_t m_id = 0;
        };

    }
}