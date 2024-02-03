#pragma once
#include <core/Play/PlayBG/IPlayBG.hpp>
#include <Siv3D/TextureRegion.hpp>

namespace ct
{
	class MonochroBG : public IPlayBG
	{
	public:
		MonochroBG(const s3d::Color& color = s3d::Palette::White);
	private:
		void init(const NotesData& notes)override;
		void apply(double count)const override;
	private:
		s3d::Color m_color;
	};
}