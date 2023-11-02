#pragma once
#include <Siv3D/String.hpp>

namespace ct
{
	class SceneInfo
	{
	public:
		static void Draw(const s3d::String& str);
	private:
		SceneInfo() = delete;
	};
}
