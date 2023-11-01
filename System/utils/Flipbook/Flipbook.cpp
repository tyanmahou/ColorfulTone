#include <utils/Flipbook/Flipbook.hpp>
#include <Siv3D.hpp>

namespace ct
{
	class Flipbook::Impl
	{
	public:

		Impl(const Texture& texture, const Point size, s3d::int32 maxSize, bool loop, const Fps& fps) :
			m_texture(texture),
			m_size(size),
			m_index(0),
			m_maxSize(maxSize),
			m_loop(loop),
			m_isEnd(false),
			m_baseFps(fps)
		{}


		Impl(const Texture& texture, const Point size, bool loop, const Fps& fps) :
			Impl(texture, size, size.x* size.y, loop, fps)
		{}

		void update(double dt)
		{
			m_totalTime += dt;
			m_index = static_cast<s3d::int32>(m_baseFps.frame(m_totalTime));
			if (m_index >= m_maxSize) {
				m_isEnd = true;

				if (m_loop) {
					m_index = m_index % m_maxSize;
				} else {
					m_index = m_maxSize - 1;
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
			).rotated(angle);
		}
		const TextureRegion scale(double scale)const
		{
			return m_texture.uv(
				(m_index) % m_size.x / static_cast<double>(m_size.x),
				(m_index) / m_size.x / static_cast<double>(m_size.y),
				1.0 / m_size.x,
				1.0 / m_size.y
			).scaled(scale);
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
	private:
		Texture m_texture;
		const Point m_size;
		const s3d::int32 m_maxSize;
		int m_index;
		bool m_loop;
		bool m_isEnd;
		Fps m_baseFps;
		double m_totalTime = 0.0;
	};

	Flipbook::Flipbook(const s3d::Texture& texture, const s3d::Point size, s3d::int32 maxSize, bool loop, const Fps& fps) :
		m_pImpl(std::make_shared<Impl>(texture, size, maxSize, loop, fps))
	{}
	Flipbook::Flipbook(const s3d::Texture& texture, const s3d::Point size, bool loop, const Fps& fps) :
		m_pImpl(std::make_shared<Impl>(texture, size, loop, fps))
	{}
	void Flipbook::update(double dt) const
	{
		m_pImpl->update(dt);
	}
	const TexturedQuad Flipbook::rotate(double angle)const
	{
		return m_pImpl->rotate(angle);
	}
	const TextureRegion Flipbook::scale(double scale)const
	{
		return m_pImpl->scale(scale);
	}

	const bool Flipbook::isEnd() const
	{
		return m_pImpl->isEnd();
	}

	void Flipbook::setLoop(bool loop)
	{
		m_pImpl->setLoop(loop);
	}

	void Flipbook::draw(const Vec2& pos, const Color& color) const
	{
		m_pImpl->draw(pos, color);
	}
	void Flipbook::drawAt(const Vec2& pos, const Color& color)const
	{
		m_pImpl->drawAt(pos, color);
	}
}
