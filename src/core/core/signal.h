#pragma  once


// thanks to http://simmesimme.github.io/tutorials/2015/09/20/signal-slot for the signals !

#include <functional>
#include <map>

namespace rain::core
{
    template <typename... Args>
    class signal
    {
    public:

        signal() : current_id(0) {}
        signal(signal const& other) : current_id(0) {}

        template <typename T>
        int connect_member(T* inst, void (T::*func)(Args...))
        {
            return connect([=](Args... args)
            {
                (inst->*func)(args...);
            });
        }

        template <typename T>
        int connect_member(T* inst, void (T::*func)(Args...) const)
        {
            return connect([=](Args... args)
            {
                (inst->*func)(args...);
            });
        }

        i32 connect(std::function<void(Args...)> const& slot) const
        {
            slots.insert(std::make_pair(++current_id, slot));
            return current_id;
        }

        void disconnect(i32 id) const
        {
            slots.erase(id);
        }

        void disconnect_all() const
        {
            slots.clear();
        }

        void emit(Args... p)
        {
            for (auto it : slots)
            {
                it.second(p...);
            }
        }

    private:
        mutable std::map<i32, std::function<void(Args...)>> slots;
        mutable i32 current_id;
    };

}
