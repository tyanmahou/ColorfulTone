#include <core/Input/PlayKey.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace ct
{
	class PlayKey::Impl
	{
	public:
		InputGroup m_Red = KeyF | KeyD | KeyS;
		InputGroup m_Blue = KeySpace | KeyC | KeyM;
		InputGroup m_Yellow = KeyJ | KeyK | KeyL;

		InputGroup m_Up = InputGroup(KeyUp);
		InputGroup m_Down = InputGroup(KeyDown);
		InputGroup m_Right = InputGroup(KeyRight);
		InputGroup m_Left = InputGroup(KeyLeft);

		InputGroup m_Start = InputGroup(KeyEnter);
		InputGroup m_BigBack = InputGroup(KeyEscape);
		InputGroup m_SmallBack = InputGroup(KeyBackspace);

		bool init()
		{
			this->update();

			return true;
		}
		void update()
		{
			auto& c = Game::Config();
			m_Red = c.m_red1 | c.m_red2 | c.m_red3;
			m_Blue = c.m_blue1 | c.m_blue2 | c.m_blue3;
			m_Yellow = c.m_yellow1 | c.m_yellow2 | c.m_yellow3;

			m_Up = KeyUp | c.m_up;
			m_Down = KeyDown | c.m_down;
			m_Left = KeyLeft | c.m_left;
			m_Right = KeyRight | c.m_right;

			m_Start = KeyEnter | c.m_start;
			m_SmallBack = KeyBackspace | c.m_smallBack;
			m_BigBack = KeyEscape | c.m_bigBack;
		}

		void reset()
		{
			auto& c = Game::Config();

			c.m_red1 = KeyF;
			c.m_red2 = KeyD;
			c.m_red3 = KeyS;
			c.m_blue1 = KeySpace;
			c.m_blue2 = KeyC;
			c.m_blue3 = KeyM;

			c.m_yellow1 = KeyJ;
			c.m_yellow2 = KeyK;
			c.m_yellow3 = KeyL;

			c.m_up = Input{};
			c.m_down = Input{};
			c.m_left = Input{};
			c.m_right = Input{};
			c.m_start = Input{};
			c.m_smallBack = Input{};
			c.m_bigBack = Input{};

			this->update();
		}
	};

	// キーの初期化
	bool PlayKey::Init()
	{
		return Instance()->m_pImpl->init();
	}

	// 変更したキーの更新
	PlayKey::PlayKey() :
		m_pImpl(std::make_unique<Impl>())
	{
	}

	void PlayKey::Update()
	{
		return Instance()->m_pImpl->update();
	}
	// 変更したキーをデフォルトの値に戻す
	void PlayKey::Reset()
	{
		return Instance()->m_pImpl->reset();
	}

	const s3d::InputGroup& PlayKey::Red()
	{
		return Instance()->m_pImpl->m_Red;
	}

	const s3d::InputGroup& PlayKey::Blue()
	{
		return Instance()->m_pImpl->m_Blue;
	}

	const s3d::InputGroup& PlayKey::Yellow()
	{
		return Instance()->m_pImpl->m_Yellow;
	}

	const s3d::InputGroup& PlayKey::Up()
	{
		return Instance()->m_pImpl->m_Up;
	}

	const s3d::InputGroup& PlayKey::Down()
	{
		return Instance()->m_pImpl->m_Down;
	}

	const s3d::InputGroup& PlayKey::Right()
	{
		return Instance()->m_pImpl->m_Right;
	}

	const s3d::InputGroup& PlayKey::Left()
	{
		return Instance()->m_pImpl->m_Left;
	}

	const s3d::InputGroup& PlayKey::Start()
	{
		return Instance()->m_pImpl->m_Start;
	}

	const s3d::InputGroup& PlayKey::BigBack()
	{
		return Instance()->m_pImpl->m_BigBack;
	}

	const s3d::InputGroup& PlayKey::SmallBack()
	{
		return Instance()->m_pImpl->m_SmallBack;
	}
}