#pragma once
#include"PlayKey.h"
#include"Useful.hpp"

class PlayKey::Impl
{
public:
	KeyCombination m_Red = Input::KeyF | Input::KeyD | Input::KeyS;
	KeyCombination m_Blue = Input::KeySpace | Input::KeyC | Input::KeyM;
	KeyCombination m_Yellow = Input::KeyJ | Input::KeyK | Input::KeyL;

	KeyCombination m_Up = KeyCombination(Input::KeyUp);
	KeyCombination m_Down = KeyCombination(Input::KeyDown);
	KeyCombination m_Right = KeyCombination(Input::KeyRight);
	KeyCombination m_Left = KeyCombination(Input::KeyLeft);

	KeyCombination m_Start = KeyCombination(Input::KeyEnter);
	KeyCombination m_BigBack = KeyCombination(Input::KeyEscape);
	KeyCombination m_SmallBack = KeyCombination(Input::KeyBackspace);
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

		m_Up = Input::KeyUp | c.m_up;
		m_Down = Input::KeyDown | c.m_down;
		m_Left = Input::KeyLeft | c.m_left;
		m_Right = Input::KeyRight | c.m_right;

		m_Start = Input::KeyEnter | c.m_start;
		m_SmallBack = Input::KeyBackspace | c.m_smallBack;
		m_BigBack = Input::KeyEscape | c.m_bigBack;
	}

	void reset()
	{
		auto& c = Game::Config();

		c.m_red1 = Input::KeyF;
		c.m_red2 = Input::KeyD;
		c.m_red3 = Input::KeyS;
		c.m_blue1 = Input::KeySpace;
		c.m_blue2 = Input::KeyC;
		c.m_blue3 = Input::KeyM;

		c.m_yellow1 = Input::KeyJ;
		c.m_yellow2 = Input::KeyK;
		c.m_yellow3 = Input::KeyL;

		c.m_up = Key(0x00, Key::VirtualKeyCode{});
		c.m_down = Key(0x00, Key::VirtualKeyCode{});
		c.m_left = Key(0x00, Key::VirtualKeyCode{});
		c.m_right = Key(0x00, Key::VirtualKeyCode{});
		c.m_start = Key(0x00, Key::VirtualKeyCode{});
		c.m_smallBack = Key(0x00, Key::VirtualKeyCode{});
		c.m_bigBack = Key(0x00, Key::VirtualKeyCode{});

		this->update();
	}
};


//-----------------------------------------------------------
//関数：Init
//-----------------------------------------------------------
//概要：キーの初期化
//-----------------------------------------------------------

bool PlayKey::Init()
{
	return Instance()->m_pImpl->init();
}

//-----------------------------------------------------------
//関数：Update
//-----------------------------------------------------------
//概要：変更したキーの更新
//-----------------------------------------------------------

PlayKey::PlayKey():
	m_pImpl(std::make_shared<Impl>())
{
}

void PlayKey::Update()
{
	return Instance()->m_pImpl->update();
}
//-----------------------------------------------------------
//関数：Reset
//-----------------------------------------------------------
//概要：変更したキーをデフォルトの値に戻す
//-----------------------------------------------------------
void PlayKey::Reset()
{
	return Instance()->m_pImpl->reset();
}

const s3d::KeyCombination & PlayKey::Red()
{
	return Instance()->m_pImpl->m_Red;
}

const s3d::KeyCombination & PlayKey::Blue()
{
	return Instance()->m_pImpl->m_Blue;
}

const s3d::KeyCombination & PlayKey::Yellow()
{
	return Instance()->m_pImpl->m_Yellow;
}

const s3d::KeyCombination & PlayKey::Up()
{
	return Instance()->m_pImpl->m_Up;
}

const s3d::KeyCombination & PlayKey::Down()
{
	return Instance()->m_pImpl->m_Down;
}

const s3d::KeyCombination & PlayKey::Right()
{
	return Instance()->m_pImpl->m_Right;
}

const s3d::KeyCombination & PlayKey::Left()
{
	return Instance()->m_pImpl->m_Left;
}

const s3d::KeyCombination & PlayKey::Start()
{
	return Instance()->m_pImpl->m_Start;
}

const s3d::KeyCombination & PlayKey::BigBack()
{
	return Instance()->m_pImpl->m_BigBack;
}

const s3d::KeyCombination & PlayKey::SmallBack()
{
	return Instance()->m_pImpl->m_SmallBack;
}
