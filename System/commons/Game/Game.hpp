#pragma once
#include <Siv3D/Fwd.hpp>
#include <Siv3D/Array.hpp>
#include <memory>
#include <utils/Singleton/DynamicSingleton.hpp>
#include <utils/Version/Version.hpp>

namespace ct
{
	class GameConfig;
	class MusicData;
	class CourseData;
	class TapSE;

	//ゲームクラス
	class Game : protected DynamicSingleton<Game>
	{
		friend DynamicSingleton<Game>;
	public:
		static constexpr Version Version{3,3,0};
	public:
		static s3d::Array<MusicData>& Musics();
		static s3d::Array<CourseData>& Courses();
		static s3d::Array<TapSE>& TapSEs();
		static GameConfig& Config();

		static void Startup();
		static bool Update();	//更新と描画
		static void Shutdown();
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;

		Game();
		~Game();
	};

}
