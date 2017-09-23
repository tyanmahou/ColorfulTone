#pragma once
#include<Siv3D.hpp>
#include<stack>
#include"PlayKey.h"
#include"SoundManager.h"
class Config
{

private:
	unsigned m_select = 0;
	unsigned m_default = 0;
	bool m_hasOnEnterd = false;
	String m_name;
	Array<std::pair<String, std::function<void()>>> m_actions;
public:
	Config() :
		m_name(L"None")
	{}
	Config(const String& name) :
		m_name(name)
	{}
	void setName(const String& name)
	{
		m_name = name;
	}
	const String& getName()const
	{
		return m_name;
	}

	void update()
	{
		if (m_actions.empty())
		{
			return;
		}
		if (m_hasOnEnterd)
		{
			if (PlayKey::Start().clicked)
			{
				SoundManager::SE::Play(L"select");

				m_actions.at(0).second();
			}
			return;
		}
		if (m_select > 0 && Input::KeyLeft.clicked)
		{
			SoundManager::SE::Play(L"select");
			m_select--;
			m_actions.at(m_select).second();
		}

		if (m_select < m_actions.size() - 1 && Input::KeyRight.clicked)
		{
			SoundManager::SE::Play(L"select");
			m_select++;
			m_actions.at(m_select).second();
		}

	}

	bool add(const String& text, std::function<void()> func)
	{
		m_actions.emplace_back(text, func);

		return true;
	}
	void applyOnEnterd(std::function<void()> func)
	{
		m_actions.clear();
		m_select = 0;
		m_default = 0;
		m_actions.emplace_back(L"Func", func);
		m_hasOnEnterd = true;
	}
	bool init(const String& text)
	{
		unsigned i = 0;
		for (auto&& a : m_actions)
		{
			if (text == a.first)
			{
				m_select = i;
				return true;
			}
			++i;
		}
		m_select = m_default;
		return false;
	}

	void setDefault(const String& text)
	{
		unsigned i = 0;
		for (auto&& a : m_actions)
		{
			if (text == a.first)
			{
				m_default = i;
				return;
			}
			++i;
		}
	}
	void draw(double y, float alpha = 1.0)const
	{
		Rect(150, y - 50, 500, 90).draw({ ColorF(1,0.6,0.2, alpha),ColorF(0.2, alpha),ColorF(0, alpha),ColorF(0, alpha) });
		FontAsset(L"configTitle")(m_name).draw(160, y - 40, ColorF(1, alpha));

		if (m_hasOnEnterd)return;

		if (m_actions.empty())
		{
			return;
		}

		if (m_select > 0)
			FontAsset(L"configSelect")(L"Å©").drawCenter(500 - 130, y);

		if (m_select < m_actions.size() - 1)
			FontAsset(L"configSelect")(L"Å®").drawCenter(500 + 130, y);

		FontAsset(L"configSelect")(m_actions.at(m_select).first).drawCenter(500, y);
	}
};

class ConfigManager;

class IConfigHierchy
{
	ConfigManager* m_manager;
protected:
	s3d::Array<Config> m_configs;

	int m_select = 0;
public:

	virtual bool update()
	{
		if (m_select > 0)
		{
			if (PlayKey::Up().clicked)
			{
				SoundManager::SE::Play(L"select");
				--m_select;
			}
		}
		if (m_select < m_configs.size() - 1)
		{
			if (PlayKey::Down().clicked)
			{
				SoundManager::SE::Play(L"select");
				++m_select;
			}
		}
		m_configs.at(m_select).update();

		return !PlayKey::SmallBack().clicked;
	}
	virtual void draw()const
	{
		TextureAsset(L"label").draw();
		FontAsset(L"label")(L"CONFIG").draw(10, 33);

		int size = static_cast<int>(m_configs.size());
		double offset = 0;

		if (m_select < size - 4)
		{
			offset = 110 * m_select;
		}
		else
		{
			offset = 110 * Max(0,size - 4);
		}
		for (unsigned i = 0; i < size; ++i)
		{
			const float alpha = i == m_select ? 1 : 0.5;
			m_configs.at(i).draw(150 + 110 * i - offset, alpha);
		}

	}
	void setManager(ConfigManager* manager)
	{
		m_manager = manager;
	}
	template<class T>
	void changePush()
	{
		m_manager->changePush<T>();
	}
};
class ConfigManager
{
private:
	std::shared_ptr<IConfigHierchy> m_config;
	std::stack<std::shared_ptr<IConfigHierchy>> m_stack;
public:
	void update()
	{
		if (!m_config)return;

		if (!m_config->update())
		{
			if (!m_stack.empty())
			{
				SoundManager::SE::Play(L"desisionSmall");
				m_config = m_stack.top();
				m_stack.pop();
			}
		}
	}
	void draw()const
	{
		if (!m_config)return;

		m_config->draw();
	}
	template<class T>
	void changePush()
	{
		if (m_config)
		{
			m_stack.push(m_config);
		}
		m_config = std::make_shared<T>();
		m_config->setManager(this);
	}
};