#pragma once

#include "thread_safe_queue.h"

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace rain::core
{
    class thread_pool
    {
    private:
        class i_thread_task
        {
        public:
            i_thread_task(void) = default;
            virtual ~i_thread_task(void) = default;
            i_thread_task(const i_thread_task& rhs) = delete;
            i_thread_task& operator=(const i_thread_task& rhs) = delete;
            i_thread_task(i_thread_task&& other) = default;
            i_thread_task& operator=(i_thread_task&& other) = default;

            /**
             * Run the task.
             */
            virtual void execute() = 0;
        };

        template <typename Func>
        class threak_task : public i_thread_task
        {
        public:
            threak_task(Func&& func)
                :m_func{ std::move(func) }
            {
            }

            ~threak_task(void) override = default;
            threak_task(const threak_task& rhs) = delete;
            threak_task& operator=(const threak_task& rhs) = delete;
            threak_task(threak_task&& other) = default;
            threak_task& operator=(threak_task&& other) = default;

            /**
             * Run the task.
             */
            void execute() override
            {
                m_func();
            }

        private:
            Func m_func;
        };

    public:

        template <typename T>
        class task_future
        {
        public:
            task_future(std::future<T>&& future)
                :m_future{ std::move(future) }
            {
            }

            task_future(const task_future& rhs) = delete;
            task_future& operator=(const task_future& rhs) = delete;
            task_future(task_future&& other) = default;
            task_future& operator=(task_future&& other) = default;
            ~task_future(void)
            {
                if (m_future.valid())
                {
                    m_future.get();
                }
            }

            auto get(void)
            {
                return m_future.get();
            }


        private:
            std::future<T> m_future;
        };

    public:

        thread_pool(void)
            :thread_pool{ std::max(std::thread::hardware_concurrency(), 2u) - 1u }
        {
            /*
             * Always create at least one thread.  If hardware_concurrency() returns 0,
             * subtracting one would turn it to UINT_MAX, so get the maximum of
             * hardware_concurrency() and 2 before subtracting 1.
             */
        }

        explicit thread_pool(const std::uint32_t numThreads)
            :m_done{ false },
            m_workQueue{},
            m_threads{}
        {
            try
            {
                for (std::uint32_t i = 0u; i < numThreads; ++i)
                {
                    m_threads.emplace_back(&thread_pool::worker, this);
                }
            }
            catch (...)
            {
                destroy();
                throw;
            }
        }

        ~thread_pool(void)
        {
            destroy();
        }

        thread_pool(const thread_pool& rhs) = delete;
        thread_pool& operator=(const thread_pool& rhs) = delete;

        template <typename Func, typename... Args>
        auto submit(Func&& func, Args&&... args)
        {
            auto boundTask = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
            using result_t = std::result_of_t<decltype(boundTask)()>;
            using packaged_task = std::packaged_task<result_t()>;
            using task_t = threak_task<packaged_task>;

            packaged_task task{ std::move(boundTask) };
            task_future<result_t> result{ task.get_future() };
            m_workQueue.push(std::make_unique<task_t>(std::move(task)));
            return result;
        }

    private:

        void worker(void)
        {
            while (!m_done)
            {
                std::unique_ptr<i_thread_task> pTask{ nullptr };
                if (m_workQueue.wait_pop(pTask))
                {
                    pTask->execute();
                }
            }
        }

        /**
         * Invalidates the queue and joins all running threads.
         */
        void destroy(void)
        {
            m_done = true;
            m_workQueue.invalidate();
            for (auto& thread : m_threads)
            {
                if (thread.joinable())
                {
                    thread.join();
                }
            }
        }

    private:
        std::atomic_bool m_done;
        tf_queue<std::unique_ptr<i_thread_task>> m_workQueue;
        std::vector<std::thread> m_threads;
    };

    namespace default_thread_pool
    {
        inline thread_pool& get_pool(void)
        {
            static thread_pool defaultPool;
            return defaultPool;
        }

        template <typename Func, typename... Args>
        inline auto submit_job(Func&& func, Args&&... args)
        {
            return get_pool().submit(std::forward<Func>(func), std::forward<Args>(args)...);
        }
    }
}