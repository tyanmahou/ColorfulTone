#include"SerialImage.h"


	void SerialImage::update()
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
	const TexturedQuad SerialImage::rotate(double angle)const
	{
		return m_texture.uv((m_index) % m_size.x / static_cast<double>(m_size.x), (m_index) / m_size.x / static_cast<double>(m_size.y), 1.0 / m_size.x, 1.0 / m_size.y).rotate(angle);
	}
	const TextureRegion SerialImage::scale(double scale)const
	{
		return m_texture.uv((m_index) % m_size.x / static_cast<double>(m_size.x), (m_index) / m_size.x / static_cast<double>(m_size.y), 1.0 / m_size.x, 1.0 / m_size.y).scale(scale);
	}

	void SerialImage::draw(const Vec2& pos,const Color& color) const
	{

		m_texture.uv((m_index) % m_size.x / static_cast<double>(m_size.x), (m_index) / m_size.x / static_cast<double>(m_size.y), 1.0 / m_size.x, 1.0 / m_size.y).draw(pos,color);
	}
	void SerialImage::drawAt(const Vec2& pos, const Color& color)const
	{
		m_texture.uv((m_index) % m_size.x / static_cast<double>(m_size.x), (m_index) / m_size.x / static_cast<double>(m_size.y), 1.0 / m_size.x, 1.0 / m_size.y).drawAt(pos, color);
	}
