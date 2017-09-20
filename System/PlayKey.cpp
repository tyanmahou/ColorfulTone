#pragma once
#include"PlayKey.h"
#include"Game.h"
//-----------------------------------------------------------
//静的メンバ変数定義
//-----------------------------------------------------------


KeyCombination PlayKey::m_Red = Input::KeyF | Input::KeyD | Input::KeyS;
KeyCombination PlayKey::m_Blue = Input::KeySpace | Input::KeyC | Input::KeyM;
KeyCombination PlayKey::m_Yellow = Input::KeyJ | Input::KeyK | Input::KeyL;

KeyCombination PlayKey::m_Up = KeyCombination(Input::KeyUp);
KeyCombination PlayKey::m_Down = KeyCombination(Input::KeyDown);
KeyCombination PlayKey::m_Right = KeyCombination(Input::KeyRight);
KeyCombination PlayKey::m_Left = KeyCombination(Input::KeyLeft);

KeyCombination PlayKey::m_Start = KeyCombination(Input::KeyEnter);
KeyCombination PlayKey::m_BigBack = KeyCombination(Input::KeyEscape);
KeyCombination PlayKey::m_SmallBack = KeyCombination(Input::KeyBackspace);


//-----------------------------------------------------------
//関数：Init
//-----------------------------------------------------------
//概要：キーの初期化
//-----------------------------------------------------------

bool PlayKey::Init()
{
	Update();

	return true;
}
//-----------------------------------------------------------
//関数：Save
//-----------------------------------------------------------
//概要：設定変更の保存
//-----------------------------------------------------------

void PlayKey::Save()
{


}
//-----------------------------------------------------------
//関数：Update
//-----------------------------------------------------------
//概要：変更したキーの更新
//-----------------------------------------------------------

void PlayKey::Update()
{
	auto& c = Game::Instance()->m_config;
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
//-----------------------------------------------------------
//関数：Reset
//-----------------------------------------------------------
//概要：変更したキーをデフォルトの値に戻す
//-----------------------------------------------------------
void PlayKey::Reset()
{
	auto& c = Game::Instance()->m_config;

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

	Update();
}