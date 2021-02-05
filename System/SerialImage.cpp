#include"SerialImage.h"
#include<Siv3D.hpp>

class SerialImage::Impl
{
	Texture m_texture;
	const Point m_size;
	const int m_maxSize;
	const int m_frame;
	int m_index;
	int m_count;
	bool m_loop;
	bool m_isEnd;
public:

	Impl::Impl(const Texture& texture, const Point size, int maxSize, int frame, bool loop = true) :
		m_texture(texture),
		m_size(size),
		m_frame(frame),
		m_index(0),
		m_count(0),
		m_maxSize(maxSize),
		m_loop(loop),
		m_isEnd(false) {}


	Impl::Impl(const Texture& texture, const Point size, int frame, bool loop = true) :
		Impl(texture, size, size.x*size.y, frame, loop)
	{}

	void update()
	{
		++m_count;

		if (m_count > m_frame)
		{
			m_count = 0;
			++m_index;
			if (m_index == m_maxSize)
			{
				m_isEnd = true;
			}
			if (m_index >= m_maxSize)
			{
				if (m_loop)
					m_index = 0;
				else
					m_index = m_maxSize;
			}
		}
	}
	const TexturedQuad rotate(double angle)const
	{
		return m_texture.uv(
			(m_index) % m_size.x / static_cast<double>(m_size.x),
			(m_index) / m_size.x / static_cast<double>(m_size.y), 
			1.0 / m_size.x,
			1.0 / m_size.y
		).rotate(angle);
	}
	const TextureRegion scale(double scale)const
	{
		return m_texture.uv(
			(m_index) % m_size.x / static_cast<double>(m_size.x), 
			(m_index) / m_size.x / static_cast<double>(m_size.y), 
			1.0 / m_size.x, 
			1.0 / m_size.y
		).scale(scale);
	}
	const bool isEnd()const 
	{
		return m_isEnd; 
	}
	void setLoop(bool loop)
	{
		m_loop = loop;
	}
	void draw(const Vec2& pos, const Color& color = Palette::White) const
	{
		m_texture.uv(
			(m_index) % m_size.x / static_cast<double>(m_size.x),
			(m_index) / m_size.x / static_cast<double>(m_size.y),
			1.0 / m_size.x, 
			1.0 / m_size.y
		).draw(pos, color);
	}
	void drawAt(const Vec2& pos, const Color& color = Palette::White)const
	{
		m_texture.uv(
			(m_index) % m_size.x / static_cast<double>(m_size.x), 
			(m_index) / m_size.x / static_cast<double>(m_size.y),
			1.0 / m_size.x,
			1.0 / m_size.y
		).drawAt(pos, color);
	}
};

SerialImage::SerialImage(const s3d::Texture & texture, const s3d::Point size, int maxSize, int frame, bool loop):
	m_pImpl(std::make_shared<Impl>(texture,size,maxSize,frame,loop))
{}
SerialImage::SerialImage(const s3d::Texture & texture, const s3d::Point size, int frame, bool loop):
	m_pImpl(std::make_shared<Impl>(texture, size, frame, loop))
{}
void SerialImage::update() const
{
	m_pImpl->update();
}
const TexturedQuad SerialImage::rotate(double angle)const
{
	return m_pImpl->rotate(angle);
}
const TextureRegion SerialImage::scale(double scale)const
{
	return m_pImpl->scale(scale);
}

const bool SerialImage::isEnd() const
{
	return m_pImpl->isEnd();
}

void SerialImage::setLoop(bool loop)
{
	m_pImpl->setLoop(loop);
}

void SerialImage::draw(const Vec2& pos, const Color& color) const
{
	m_pImpl->draw(pos, color);
}
void SerialImage::drawAt(const Vec2& pos, const Color& color)const
{
	m_pImpl->drawAt(pos, color);
}
