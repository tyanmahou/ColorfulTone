#pragma once
#include <Siv3D/String.hpp>

namespace ct
{
	class SceneInfo
	{
	public:
		static void Header(const s3d::String& str);
		static void Draw(const s3d::String& str);

		static void DrawBack(const s3d::ColorF& color = s3d::Palette::White);
	private:
		SceneInfo() = delete;
	};
}
