#pragma once
#include "CommonNetInclude.h"

namespace lsms
{
    namespace libnet
    {
        template <typename T>
        class TSQueue
        {
        public:
            TSQueue() = default;
            TSQueue(const TSQueue<T> &) = delete;
            virtual ~TSQueue() { clear(); };
            
        public:
            const T &front()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.front();
            }

            const T &back()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.back();
            }

            void push_front(const T &item)
            {
                std::scoped_lock lock(muxQueue);
                deqQueue.emplace_front(std::move(item));
            }

            void push_back(const T &item)
            {
                std::scoped_lock lock(muxQueue);
                deqQueue.emplace_back(std::move(item));
            }

            bool empty()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.empty();
            }
    
            size_t count()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.size();
            }

            void clear()
            {
                std::scoped_lock lock(muxQueue);
                deqQueue.clear();
            }

            T pop_front()
            {
                std::scoped_lock lock(muxQueue);
                auto t = std::move(deqQueue.front());
                deqQueue.pop_front();
                return t;
            }

            T pop_back()
            {
                std::scoped_lock lock(muxQueue);
                auto t = std::move (deqQueue.back);
                deqQueue.pop_back();
                return t;
            }

        protected:
            std::mutex muxQueue;
            std::deque<T> deqQueue;
        };
    }
}