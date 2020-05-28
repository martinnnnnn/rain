#pragma once



namespace rain::core
{
	template<typename T>
	class Singleton
	{
	protected:
        Singleton() noexcept = default;

		Singleton(const Singleton&) = delete;

		Singleton& operator=(const Singleton&) = delete;

		virtual ~Singleton() = default;

        //static T* instance;
	public:
		inline static T* get() noexcept(std::is_nothrow_constructible<T>::value)
		{
            static T* instance = new T();

			return instance;
		}
	};

}