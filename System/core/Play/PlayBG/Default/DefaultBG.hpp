#pragma once
#include <core/Play/PlayBG/IPlayBG.hpp>
#include <Siv3D/TextureRegion.hpp>

namespace ct
{
	class DefaultBG : public IPlayBG
	{
	public:
		DefaultBG() = default;
	private:
		void init(const NotesData& notes)override;
		void apply(double count)const override;
	private:
		s3d::TextureRegion m_texture;
	};
}