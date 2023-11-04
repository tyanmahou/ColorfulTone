#pragma once
#include <memory>

namespace ct
{
	class KeyConfigManager
	{
	public:
		class Impl;
	private:

		std::shared_ptr<Impl> m_pImpl;
	public:

		KeyConfigManager();

		template<class Type>
		void changeMode();

		bool update();

		void draw()const;

	};
}