#include <scenes/utils/SceneInfo.hpp>
#include <commons/FontName.hpp>
#include <Siv3D.hpp>

namespace ct
{
	void SceneInfo::Header(const s3d::String& str)
	{
		//const s3d::int32 h = 40;
		//Rect(0, 600 - h, 800, h).draw(Palette::Black);
		const auto width = FontAsset(FontName::Info)(str).region().w;
		FontAsset(FontName::Info)(str).draw(790 - width, 5);
	}
	void SceneInfo::Draw(const s3d::String& str)
	{
		const s3d::int32 h = 40;
		Rect(0, 600 - h, 800, h).draw(Palette::Black);
		const auto width = FontAsset(FontName::Info)(str).region().w;
		FontAsset(FontName::Info)(str).draw(790 - width, 600 - h + 10);
	}
	void SceneInfo::DrawBack(const s3d::ColorF& color)
	{
		TextureAsset(U"backIcon").draw(color);
		auto d = FontAsset(FontName::Info)(U"ESC");
		d.draw(14, 5, color);
	}
}
