#include <scenes/utils/SceneInfo.hpp>
#include <commons/FontName.hpp>
#include <Siv3D.hpp>

namespace ct
{
	void SceneInfo::Draw(const s3d::String& str)
	{
		const int h = 40;
		Rect(0, 600 - h, 800, h).draw(Palette::Black);
		const auto width = FontAsset(FontName::Info)(str).region().w;
		FontAsset(FontName::Info)(str).draw(780 - width, 600 - h + 10);
	}
}
