#pragma once
#include"Singleton.h"
#include<memory>

using NoteType = int;

class AutoPlayManager final : protected Singleton<AutoPlayManager>
{
private:
	class Impl;
	std::shared_ptr<Impl> m_pImpl;

	AutoPlayManager();
public:

	static void ChangePlayMode();
	static void SetAutoPlay(bool isAuto);
	static bool IsAutoPlay();
	static bool IsRedPressed();
	static bool IsBluePressed();
	static bool IsYellowPressed();
	static void Update();
	static void Input(NoteType type);
	friend class Singleton<AutoPlayManager>;
};
