#include"GIFTexture.h"

GIFTexture::GIFTexture(const FilePath& path, const unsigned int frame):m_frameCount(0),m_frame(frame),m_nowImage(0)
{
	m_images= Imaging::LoadAnimatedGIF(path);

}

void GIFTexture::update()
{

	++m_frameCount;


	if (m_frameCount > m_frame)
	{
		m_frameCount= 0;
		++m_nowImage;
	}

	if (m_nowImage >= m_images.size())
		m_nowImage = 0;


	m_texture.fill(m_images[m_nowImage]);

}
void GIFTexture::draw()const
{

	if (m_images.size() == 0)
		return;

	m_texture.draw();

}