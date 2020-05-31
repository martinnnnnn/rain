#pragma once

#include <unordered_map>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <utility>

namespace rain::core
{
	template<class _Kty,
		class _Ty,
		class _Hasher = std::hash<_Kty>,
		class _Keyeq = std::equal_to<_Kty>,
		class _Alloc = std::allocator<std::pair<const _Kty, _Ty>>>
    class unordered_map_tf
    {
    public:
        
        ~unordered_map_tf(void)
        {
            invalidate();
        }

        /**
         * Attempt to get the first value in the queue.
         * Returns true if a value was successfully written to the out parameter, false otherwise.
         */
        bool try_at(_Kty key, _Ty& out)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };

			if (m_map.find(key) == m_map.end() || !m_valid)
			{
				return false;
			}

			out = m_map.at(key);
            return true;
        }

        /**
         * Get the first value in the queue.
         * Will block until a value is available unless clear is called or the instance is destructed.
         * Returns true if a value was successfully written to the out parameter, false otherwise.
         */
        bool wait_at(_Kty key, _Ty& out) const
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            m_condition.wait(lock, [this]()
            {
                return m_map.find(key) != m_map.end() || !m_valid;
            });
            /*
             * Using the condition in the predicate ensures that spurious wakeups with a valid
             * but empty queue will not proceed, so only need to check for validity before proceeding.
             */
            if (!m_valid)
            {
                return false;
            }
			out = m_map.at(key);
            return true;
        }

        /**
         * Adds a new value to the map.
         */
        void add(_Kty key, _Ty& out)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
			m_map[key] = out;
        }

        /**
         * Check whether or not the map contains the key.
         */
        bool contains(_Kty key) const
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            return m_map.find(key) != m_map.end();
        }

        /**
         * Clear all items from the map.
         */
        void clear(void)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
			m_map.empty();
            m_condition.notify_all();
        }

        /**
         * Invalidate the queue.
         * Used to ensure no conditions are being waited on in waitPop when
         * a thread or the application is trying to exit.
         * The queue is invalid after calling this method and it is an error
         * to continue using a queue after this method has been called.
         */
        void invalidate(void)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            m_valid = false;
            m_condition.notify_all();
        }

        /**
         * Returns whether or not this queue is valid.
         */
        bool is_valid(void) const
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            return m_valid;
        }

    private:
        std::atomic_bool m_valid{ true };
        mutable std::mutex m_mutex;
        std::unordered_map<_Ty, _Hasher, _Keyeq, _Alloc> m_map;
        std::condition_variable m_condition;
    };
}