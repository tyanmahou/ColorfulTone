#pragma once
#include <memory>
#include <Siv3D/InputGroups.hpp>
#include <utils/Singleton/DynamicSingleton.hpp>

namespace ct
{
	class PlayKey :protected DynamicSingleton<PlayKey>
	{
		friend class DynamicSingleton<PlayKey>;
		friend class PlayKeyConfig;
		friend class DetailKeyConfig;
	public:

		static const s3d::InputGroup& Red();
		static const s3d::InputGroup& Blue();
		static const s3d::InputGroup& Yellow();

		static const s3d::InputGroup& Up();
		static const s3d::InputGroup& Down();
		static const s3d::InputGroup& Right();
		static const s3d::InputGroup& Left();

		static const s3d::InputGroup& Start();
		static const s3d::InputGroup& BigBack();
		static const s3d::InputGroup& SmallBack();

		//キーの初期化
		static bool Init();
	private:
		PlayKey();

		//キー組合せの更新
		static void Update();
		//キーの初期化
		static void Reset();

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}
