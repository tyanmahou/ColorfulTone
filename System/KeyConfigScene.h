#pragma once

#include "Game.h"

#include"KeyConfig.h"

class Config
{

private:
	unsigned m_select = 0;
	unsigned m_default = 0;

	String m_name;
	Array<std::pair<String,std::function<void()>>> m_actions;
public:
	Config():
		m_name(L"None")
	{

	}
	Config(const String& name):
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

		if (m_select>0&&Input::KeyLeft.clicked)
		{
			SoundAsset(L"select").playMulti(0.5);
			m_select--;
			m_actions.at(m_select).second();
		}

		if (m_select<m_actions .size()-1 && Input::KeyRight.clicked)
		{
			SoundAsset(L"select").playMulti(0.5);
			m_select++;
			m_actions.at(m_select).second();
		}

	}

	bool add(const String& text,std::function<void()> func)
	{
		m_actions.emplace_back(text, func);

		return true;
	}
	bool init(const String& text)
	{
		unsigned i = 0;
		for (auto&& a: m_actions)
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
	void draw(const Font& font,double y)const
	{
		if (m_actions.empty())
		{
			return;
		}

		if(m_select>0)
		font(L"←").drawCenter(500-130, y);
		
		if (m_select < m_actions.size()-1)
			font(L"→").drawCenter(500 + 130, y);

		font(m_actions.at(m_select).first).drawCenter(500,y);
	}
};

//仮実装
//ディレクリィ構造みたいにできんかな？
class TapSEConfig
{
	enum Mode :unsigned int
	{
		Perfect,
		Great,
		Good, //コンフィグの数
		TOTAL_CONFIG //コンフィグの数
	}m_mode;
	std::array<Config, 3> m_configs;

public:
	TapSEConfig();

	bool update();
	void draw(const Font& font, const Font & configFont) const;
};

class ConfigScene :public MyApp::Scene
{

private:
	Font m_font;
	Font m_configFont;

	int m_timer = 0;

	TapSEConfig m_seConfig;

	KeyConfig m_keyConfig;

	std::array<Config,3> m_configs;

	EasingController<double> m_keyConfigEasing;

	bool m_isKeyConfig=false;
	bool m_isSeConfig = false;

	enum Mode :unsigned int
	{
		ClearRate,
		CircleCut,
		PlayScale,
		TapSE,
		KeyConfig,
		TOTAL_CONFIG //コンフィグの数
	}m_mode;

public:


	ConfigScene();
	~ConfigScene();


	void update() override;
	void draw() const override;

	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
};