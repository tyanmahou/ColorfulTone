#pragma once
#include<Siv3D/Fwd.hpp>
#include"Singleton.h"
#include<memory>
class PlayKey :protected Singleton<PlayKey>
{
public:

	static const s3d::KeyCombination& Red();
	static const s3d::KeyCombination& Blue();
	static const s3d::KeyCombination& Yellow();

	static const s3d::KeyCombination& Up();
	static const s3d::KeyCombination& Down();
	static const s3d::KeyCombination& Right();
	static const s3d::KeyCombination& Left();

	static const s3d::KeyCombination& Start();
	static const s3d::KeyCombination& BigBack();
	static const s3d::KeyCombination& SmallBack();

	//キーの初期化
	static bool Init();

private:
	class Impl;
	std::shared_ptr<Impl> m_pImpl;

	PlayKey();

	//キー組合せの更新
	static void Update();
	//キーの初期化
	static void Reset();

	friend class Singleton<PlayKey>;
	friend class PlayKeyConfig;
	friend class DetailKeyConfig;
};
