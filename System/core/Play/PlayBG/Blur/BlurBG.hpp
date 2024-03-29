﻿#pragma once
#include <core/Play/PlayBG/IPlayBG.hpp>
#include <Siv3D/TextureRegion.hpp>
#include <Siv3D/RenderTexture.hpp>

namespace ct
{
	class BlurBG : public IPlayBG
	{
	public:
		BlurBG();
	private:
		void init(const NotesData& notes)override;
		void apply(double count)const override;
	private:
		TextureRegion m_texturRegion;
	};
}