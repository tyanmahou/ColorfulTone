#pragma once
#include <Siv3D.hpp>
#include <stack>

namespace ct
{
	class Config
	{
	private:
		size_t m_select = 0;
		size_t m_default = 0;
		bool m_hasOnEnterd = false;
		String m_name;
		Array<std::pair<String, std::function<void()>>> m_actions;
	public:
		Config();
		Config(const String& name);

		void setName(const String& name)
		{
			m_name = name;
		}
		const String& getName()const
		{
			return m_name;
		}

		void update();

		bool add(const String& text, std::function<void()> func);
		void applyOnEnterd(std::function<void()> func);
		bool init(const String& text);
		bool init(size_t index);

		void setDefault(const String& text);
		void draw(double y, double alpha = 1.0)const;
	};

	class ConfigManager;

	class IConfigHierchy
	{
		ConfigManager* m_manager;
	protected:
		s3d::Array<Config> m_configs;

		int m_select = 0;
	public:

		virtual bool update();
		virtual void draw()const;
		void setManager(ConfigManager* manager)
		{
			m_manager = manager;
		}
		template<class T>
		void changePush();
	};
	class ConfigManager
	{
	private:
		std::shared_ptr<IConfigHierchy> m_config;
		std::stack<std::shared_ptr<IConfigHierchy>> m_stack;
	public:
		bool update();
		void draw()const
		{
			if (!m_config)return;

			m_config->draw();
		}
		template<class T>
		void changePush()
		{
			if (m_config) {
				m_stack.push(m_config);
			}
			m_config = std::make_shared<T>();
			m_config->setManager(this);
		}

		void clear();
		void reset();
		bool isRoot()const;
	};
	template<class T>
	inline void IConfigHierchy::changePush()
	{
		m_manager->changePush<T>();
	}
}
