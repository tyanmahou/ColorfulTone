#pragma once

namespace ct
{
	/// <summary>
	/// Singleton template
	/// </summary>
	template<class Type>
	class Singleton
	{
	public:

		static Type* Instance()
		{
			static Type instance;
			return &instance;
		}
	protected:
		Singleton() {}
		virtual ~Singleton() {}
	private:
		Singleton(const Singleton& other) = delete;
		Singleton& operator=(const Singleton& other) = delete;
	};
}