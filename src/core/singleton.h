#pragma once

template<typename Entity>
class Singleton
{
protected:
    Singleton() noexcept = default;

    Singleton(const Singleton&) = delete;

    Singleton& operator=(const Singleton&) = delete;

    virtual ~Singleton() = default;

public:
    static Entity& Get() noexcept(std::is_nothrow_constructible<Entity>::value)
    {
        static Entity instance;

        return instance;
    }
};