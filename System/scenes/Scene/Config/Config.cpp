﻿#include <scenes/Scene/Config/Config.hpp>
#include <core/Input/PlayKey.hpp>
#include <commons/Audio/SoundManager.hpp>
#include <commons/FontName.hpp>
#include <scenes/utils/Util.hpp>

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
		if (!m_isActive) {
			return;
		}
		if (m_actions.empty()) {
			return;
		}
		if (m_hasOnEnterd) {
			if (PlayKey::Start().down()) {
				if (m_needEnterdSe) {
					SoundManager::PlaySe(U"select");
				}

				m_actions.at(0).callback();
			}
			return;
		}
		if (m_select > 0 && PlayKey::Left().down()) {
			SoundManager::PlaySe(U"select");
			m_select--;
			m_actions.at(m_select).callback();
		}

		if (m_select < m_actions.size() - 1 && PlayKey::Right().down()) {
			SoundManager::PlaySe(U"select");
			m_select++;
			m_actions.at(m_select).callback();
		}
	}

	bool Config::add(const String& text, std::function<void()> func, const String& detail)
	{
		m_actions.emplace_back(text, func, detail);

		return true;
	}
	void Config::applyOnEnterd(std::function<void()> func, const String& detail)
	{
		m_actions.clear();
		m_select = 0;
		m_default = 0;
		m_actions.emplace_back(U"Func", func, detail);
		m_hasOnEnterd = true;
	}
	void Config::setNeedEnterdSe(bool needEnterdSe)
	{
		m_needEnterdSe = needEnterdSe;
	}
	bool Config::init(const String& text)
	{
		for (const auto& [i, a] : Indexed(m_actions)) {
			if (text == a.name) {
				m_select = i;
				return true;
			}
		}
		m_select = m_default;
		return false;
	}
	bool Config::init(size_t index)
	{
		if (index < m_actions.size()) {
			m_select = index;
			return true;
		} else {
			m_select = m_default;
			return false;
		}
	}
	void Config::setDefault(const String& text)
	{
		size_t i = 0;
		for (auto&& a : m_actions) {
			if (text == a.name) {
				m_default = i;
				return;
			}
			++i;
		}
	}
	void Config::draw(double y, bool isSelect)const
	{
		const double alpha = m_isActive ? 1 : 0.5;

		RectF rect(150, y - 30, 500, 60);
		rect.draw({ ColorF(0.2,0.5,0.4, alpha),ColorF(0.2, alpha),ColorF(0,alpha),ColorF(0, 0, 0.1, alpha) });
		if (isSelect) {
			rect.drawFrame(1, 1, ColorF(1.0, Periodic::Sine0_1(1s) * 0.5 + 0.5));
		}
		// タイトル
		ContractionDrawbleStringCL(FontAsset(FontName::ConfigTitle)(m_name), Vec2{ 160, y - 5 }, 220, ColorF(1, alpha));

		if (!m_isActive) {
			rect.draw(ColorF(0, 0.8 * alpha));
		}
		if (m_hasOnEnterd)return;

		if (m_actions.empty()) {
			return;
		}
		if (m_isActive) {
			if (m_select > 0) {
				FontAsset(FontName::ConfigSelect)(U"◀").drawAt(500 - 110, y);
			}

			if (m_select + 1 < m_actions.size()) {
				FontAsset(FontName::ConfigSelect)(U"▶").drawAt(500 + 110, y);
			}
		}

		if (m_select < m_actions.size()) {
			ContractionDrawbleString(FontAsset(FontName::ConfigSelect)(m_actions.at(m_select).name), { 500, y }, 210);
		}
		if (m_extention && m_select < m_actions.size()) {
			m_extention(m_select, y);
		}
	}

	void Config::drawDetail() const
	{
		if (m_select >= m_actions.size()) {
			return;
		}
		const String& detail = m_actions.at(m_select).detail;
		if (detail) {
			FontAsset(FontName::Info)(detail).draw(Arg::topRight = Vec2{ 780, 532 });
		}
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

		constexpr int32 viewMax = 5;
		constexpr double yOffs = 80;
		if (m_select + viewMax < size) {
			offset = yOffs * m_select;
		} else {
			offset = yOffs * Max<s3d::int32>(0, static_cast<s3d::int32>(size) - viewMax);
		}
		for (size_t i = 0; i < size; ++i) {
			m_configs[i].draw(140 + yOffs * i - offset, static_cast<s3d::int32>(i) == m_select);
		}
		// 詳細
		if (m_select < size) {
			Transformer2D t2d(Mat3x2::Identity(), s3d::Transformer2D::Target::SetLocal);
			RectF(0, 530, 800, 70).draw(ColorF(0, this->detailAlpha()));
			m_configs[m_select].drawDetail();
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
