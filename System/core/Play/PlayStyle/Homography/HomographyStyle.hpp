#pragma once
#include <core/Play/PlayStyle/Portrait/PortraitStyle.hpp>

namespace ct
{
	// 奥レーンモード
	class HomographyStyle : public PortraitStyle
	{
		void drawPreview([[maybe_unused]] std::function<void()> drawCallback) const override;
		void drawFrame(bool red, bool blue, bool yellow, std::function<void()> drawCallback) const override;
		bool canDraw(double y) const override;
	};
}