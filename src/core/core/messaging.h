#pragma once

#include <unordered_map>
#include <vector>
#include <functional>

namespace rain::core
{
    struct base_message
    {
        virtual ~base_message() {}
    protected:
    };

    template <typename T>
    struct message : base_message
    {
        message(const T& m) : msg(m) {}
        const T& msg;
    };

    struct messager
    {
        template <class message_type>
        using call_type = std::function<void(const message_type&)>;

        template <typename message_type>
        void subscribe(call_type<message_type> callable)
        {
            u32 type = core::type_id<base_message>::get<message_type>();
            if (type >= subscribers.size())
            {
                subscribers.resize(type + 1);
            }
            subscribers[type].push_back(callback_wrapper<message_type>(callable));
        }

        template <typename message_type>
        void emit(const message_type& msg)
        {
            u32 type = core::type_id<base_message>::get<message_type>();
            if (type >= subscribers.size())
            {
                subscribers.resize(type + 1);
            }

            message<message_type> event_wrapper(msg);
            for (auto& receiver : subscribers[type])
            {
                receiver(event_wrapper);
            }
        }

        template <typename message_type>
        struct callback_wrapper
        {
            callback_wrapper(call_type<message_type> callable) : callable(callable) {}

            void operator() (const base_message& m)
            {
                callable(static_cast<const message<message_type>&>(m).msg);
            }

            call_type<message_type> callable;
        };

        std::vector<std::vector<call_type<base_message>>> subscribers;
    };
}