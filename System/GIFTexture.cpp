#include"GIFTexture.h"
#include<Siv3D.hpp>

class GIFTexture::Impl
{
private:
	unsigned int m_frame;			//何フレームで次のイメージにかわるか
	unsigned int m_frameCount;		//現在のカウント値
	unsigned int m_nowImage;		//現在の描画フレーム
	Array<Image> m_images;
	DynamicTexture m_texture;

public:
	Impl(const FilePath& path, const unsigned  int frame):
		m_frameCount(0),
		m_frame(frame),
		m_nowImage(0),
		m_images(Imaging::LoadAnimatedGIF(path))
	{}
	void update()
	{
		++m_frameCount;

		if (m_frameCount > m_frame)
		{
			m_frameCount = 0;
			++m_nowImage;
		}

		if (m_nowImage >= m_images.size())
		{
			m_nowImage = 0;
		}

		m_texture.fill(m_images[m_nowImage]);
	}
	void draw()const
	{
		if (m_images.size() == 0)
		{
			return;
		}
		m_texture.draw();
	}
};
GIFTexture::GIFTexture(const FilePath& path, const unsigned int frame):
	m_pImpl(std::make_shared<Impl>(path, frame))
{}

void GIFTexture::update()const
{
	m_pImpl->update();
}
void GIFTexture::draw()const
{
	m_pImpl->draw();
}