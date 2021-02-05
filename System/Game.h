#pragma once
#include"Singleton.h"
#include<Siv3D/Fwd.hpp>
#include<Siv3D/Array.hpp>
#include<memory>

class GameConfig;
class MusicData;
class CourseData;
class TapSE;

//ゲームクラス
class Game : protected Singleton<Game>
{
public:
	static constexpr auto Version = L"Ver3.1.1";
private:
	class Impl;
	std::shared_ptr<Impl> m_pImpl;

	Game();
public:
	static bool SetLoadCompleted(bool completed = true);
	static bool IsLoadCompleted();
	static s3d::Array<MusicData>& Musics();
	static s3d::Array<CourseData>& Courses();
	static s3d::Array<TapSE>& TapSEs();
	static GameConfig& Config();
	static bool UpdateAndDraw();	//更新と描画

	friend Singleton < Game >;
};

