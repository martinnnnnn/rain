#pragma once


#include <vector>

#include "core/types.h"

namespace rain::core
{
    struct item_container_base
    {
    };

    template<typename T>
    struct item_container : public item_container_base
    {
        std::vector<T*> data;

        void add(T* new_item)
        {
            if (data.size() == 0)
            {
                data.push_back(new_item);
                return;
            }

            for (u32 i = 0; i < data.size(); ++i)
            {
                if (data[i] == nullptr)
                {
                    data[i] = new_item;
                    return;
                }
            }

            data.push_back(new_item);
            return;
        }

        T * const create()
        {
            if (data.size() == 0)
            {
                data.push_back(new T());
                return data[0];
            }

            for (u32 i = 0; i < data.size(); ++i)
            {
                if (data[i] == nullptr)
                {
                    data[i] = new T();
                    return data[i];
                }
            }

            data.push_back(new T());
            return data[data.size() - 1];
        }

        T* const get()
        {
            for (u32 i = 0; i < data.size(); ++i)
            {
                if (data[i] != nullptr)
                {
                    return data[i];
                }
            }
            return nullptr;
        }

        void clear()
        {
            for (u32 i = 0; i < data.size(); ++i)
            {
                delete(data[i]);
                data[i] = nullptr;
            }
            data.clear();
        }

        void remove(T** item_ptr)
        {
            for (u32 i = 0; i < data.size(); ++i)
            {
                if (data[i] == *item_ptr)
                {
                    delete data[i];
                    data[i] = nullptr;
                    *item_ptr = nullptr;
                    return;
                }
            }
        }
    };

    template<typename ITEM_TYPE_ID>
    struct heterogenous_vector
    {
        using item_type_id = core::type_id<ITEM_TYPE_ID>;

        std::vector<item_container_base*> items;

        template<typename T>
        void add(T* item)
        {
            u32 t_id = item_type_id::template get<T>();
            if (t_id >= items.size())
            {
                items.resize(t_id + 1);
                items[t_id] = new item_container<T>();
            }

            return static_cast<item_container<T>*>(items[t_id])->add(item);
        }

        template<typename T>
        T * const create()
        {
            u32 t_id = item_type_id::template get<T>();

            if (t_id >= items.size())
            {
                items.resize(t_id + 1);
                items[t_id] = new item_container<T>();
            }

            return static_cast<item_container<T>*>(items[t_id])->create();
        }

        template<typename T>
        bool has_unique()
        {
            u32 t_id = item_type_id::template get<T>();
            return (t_id < items.size() && items[t_id] != nullptr);
        }

        template<typename... T>
        bool has()
        {
            bool result = true;
            using accumulator_type = bool[];
            accumulator_type accumulator = { result, (result = result && has_unique<T>())... };
            (void)accumulator;
            return result;
        }

        template<typename T>
        void remove(T** item_ptr)
        {
            u32 t_id = item_type_id::template get<T>();

            if (t_id < items.size())
            {
                static_cast<item_container<T>*>(items[t_id])->remove(item_ptr);
            }
        }

        template<typename T>
        void clear()
        {
            u32 t_id = item_type_id::template get<T>();
            if (t_id < items.size())
            {
                static_cast<item_container<T>*>(items[t_id])->clear();
            }
        }

        template<typename T>
        T * const get()
        {
            u32 t_id = item_type_id::template get<T>();
            if (t_id < items.size())
            {
                return static_cast<item_container<T>*>(items[t_id])->get();
            }
            return nullptr;
        }

        template<typename T>
        std::vector<T*>* get_all()
        {
            u32 t_id = item_type_id::template get<T>();
            if (t_id < items.size())
            {
                return &(static_cast<item_container<T>*>(items[t_id])->data);
            }
            return nullptr;
        }
    };

}