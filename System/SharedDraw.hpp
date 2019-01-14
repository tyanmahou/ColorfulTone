#pragma once
#include"Util.h"
class SharedDraw
{
public:
	template<class T>
	static void Select(
		const Array<T>& ar, 
		uint32 select,
		bool onMoveSelect,
		std::function<const String&(const T&)> strCallBack,
		uint32 width = 280,
		std::function<Color(const T&)> colorCallback = [](const T&) {return Palette::White; },
		std::function<void(const T&,Vec2 pos)> drawable = [](const T&,Vec2){}
	){
		const uint32 size = ar.size();
		if (size == 0)
		{
			return;
		}
		for (int i = 0; i < 10; ++i)
		{
			const auto offset = i == 4 ? (onMoveSelect) ? -25 : -30 : 0;
			Color uiColor = i == 4 ? Palette::Yellow : Palette::White;
			const Vec2 pos{ 430 + offset, 60 * i };
			TextureAsset(L"levelbg").draw(pos, uiColor);
			const auto index = (select + i - 4 + size * Abs(i - 4)) % size;

			const auto color = colorCallback(ar[index]);
			util::ContractionDrawbleString(
				FontAsset(L"selectMusics")(strCallBack(ar[index])),
				pos + Vec2{80,13},
				width, 
				color,
				false
			);
			drawable(ar[index],pos);
		}
		TextureAsset(L"levelMask").draw(430, 0, Palette::Black);
	}
private:
	SharedDraw()=default;
};

