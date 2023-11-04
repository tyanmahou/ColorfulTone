#include <scenes/Scene/KeyConfig/Config.hpp>
#include <core/Input/PlayKey.hpp>
#include <commons/Audio/SoundManager.hpp>

namespace ct
{
	Config::Config() :
		m_name(U"None")
	{}

	Config::Config(const String& name) :
		m_name(name)
	{}
	void Config::update()
	{
		if (m_actions.empty()) {
			return;
		}
		if (m_hasOnEnterd) {
			if (PlayKey::Start().down()) {
				SoundManager::PlaySe(U"select");

				m_actions.at(0).second();
			}
			return;
		}
		if (m_select > 0 && PlayKey::Left().down()) {
			SoundManager::PlaySe(U"select");
			m_select--;
			m_actions.at(m_select).second();
		}

		if (m_select < m_actions.size() - 1 && PlayKey::Right().down()) {
			SoundManager::PlaySe(U"select");
			m_select++;
			m_actions.at(m_select).second();
		}
	}

	bool Config::add(const String& text, std::function<void()> func)
	{
		m_actions.emplace_back(text, func);

		return true;
	}
	void Config::applyOnEnterd(std::function<void()> func)
	{
		m_actions.clear();
		m_select = 0;
		m_default = 0;
		m_actions.emplace_back(U"Func", func);
		m_hasOnEnterd = true;
	}
	bool Config::init(const String& text)
	{
		unsigned i = 0;
		for (auto&& a : m_actions) {
			if (text == a.first) {
				m_select = i;
				return true;
			}
			++i;
		}
		m_select = m_default;
		return false;
	}

	void Config::setDefault(const String& text)
	{
		unsigned i = 0;
		for (auto&& a : m_actions) {
			if (text == a.first) {
				m_default = i;
				return;
			}
			++i;
		}
	}
	void Config::draw(double y, double alpha)const
	{
		RectF(150, y - 50, 500, 90).draw({ ColorF(1,0.6,0.2, alpha),ColorF(0.2, alpha),ColorF(0, alpha),ColorF(0, alpha) });
		FontAsset(U"configTitle")(m_name).draw(160, y - 40, ColorF(1, alpha));

		if (m_hasOnEnterd)return;

		if (m_actions.empty()) {
			return;
		}

		if (m_select > 0)
			FontAsset(U"configSelect")(U"←").drawAt(500 - 130, y);

		if (m_select < m_actions.size() - 1)
			FontAsset(U"configSelect")(U"→").drawAt(500 + 130, y);

		FontAsset(U"configSelect")(m_actions.at(m_select).first).drawAt(500, y);
	}

	bool IConfigHierchy::update()
	{
		if (m_select > 0) {
			if (PlayKey::Up().down()) {
				SoundManager::PlaySe(U"select");
				--m_select;
			}
		}
		if (m_select < m_configs.size() - 1) {
			if (PlayKey::Down().down()) {
				SoundManager::PlaySe(U"select");
				++m_select;
			}
		}
		m_configs.at(m_select).update();

		return !PlayKey::SmallBack().down();
	}

	void IConfigHierchy::draw()const
	{
		size_t size = m_configs.size();
		double offset = 0;

		if (m_select + 4 < size) {
			offset = 110.0 * m_select;
		} else {
			offset = 110.0 * Max<size_t>(0, size - 4);
		}
		for (size_t i = 0; i < size; ++i) {
			const double alpha = static_cast<int>(i) == m_select ? 1 : 0.5;
			m_configs.at(i).draw(150 + 110 * i - offset, alpha);
		}
	}

	bool ConfigManager::update()
	{
		if (!m_config)return false;

		if (!m_config->update()) {
			if (!m_stack.empty()) {
				SoundManager::PlaySe(U"desisionSmall");
				m_config = m_stack.top();
				m_stack.pop();
			} else {
				return false;
			}
		}
		return true;
	}

	void ConfigManager::clear()
	{
		m_config = nullptr;
		std::stack<std::shared_ptr<IConfigHierchy>> empty;
		m_stack.swap(empty);
	}

	void ConfigManager::reset()
	{
		while (!m_stack.empty()) {
			m_config = m_stack.top();
			m_stack.pop();
		}
	}

	bool ConfigManager::isRoot() const
	{
		return m_config && m_stack.empty();
	}
}
