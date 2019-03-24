#pragma once
#include"Singleton.h"
#include<memory>

using NoteType = int;

class InputManager final : protected Singleton<InputManager>
{
private:
	class Impl;
	std::shared_ptr<Impl> m_pImpl;

	InputManager();
public:

	static bool IsRedClicked();
	static void PopRed();

	static bool IsBlueClicked();
	static void PopBlue();

	static bool IsYellowClicked();
	static void PopYellow();

	static bool IsAnyClicked();
	static void Update();

	friend class Singleton<InputManager>;
};
