#pragma once
#include"Game.h"

class GIFTexture 
{
	private:
		unsigned int m_frame;			//何フレームで次のイメージにかわるか
		unsigned int m_frameCount;		//現在のカウント値
		unsigned int m_nowImage;		//現在の描画フレーム
		Array<Image> m_images;
		DynamicTexture m_texture;

	public:
		GIFTexture(const FilePath& path, const unsigned  int frame=1);
		void update();
		void draw()const;

};