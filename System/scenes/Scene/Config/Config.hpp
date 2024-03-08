#pragma once
#include <Siv3D.hpp>
#include <stack>

namespace ct
{
	class Config
	{
	public:
		struct Action
		{
			s3d::String name;
			std::function<void()> callback;

			s3d::String detail;
		};
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

		bool add(const String& text, std::function<void()> func, const String& detail = U"");
		void applyOnEnterd(std::function<void()> func, const String& detail = U"");
		void setNeedEnterdSe(bool needEnterdSe);
		bool init(const String& text);
		bool init(size_t index);

		void setDefault(const String& text);
		void draw(double y, bool isSelect)const;

		void drawDetail() const;
	private:
		size_t m_select = 0;
		size_t m_default = 0;
		bool m_hasOnEnterd = false;
		bool m_needEnterdSe = true;
		String m_name;
		Array<Action> m_actions;
	};

	class ConfigManager;

	class IConfigHierchy
	{
		ConfigManager* m_manager;
	protected:
		s3d::Array<Config> m_configs;

		s3d::int32 m_select = 0;
	public:

		virtual bool update();
		virtual void draw()const;
		void setManager(ConfigManager* manager)
		{
			m_manager = manager;
		}
		template<class T>
		void changePush();

		virtual double detailAlpha() const
		{
			return 0.8;
		}
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
