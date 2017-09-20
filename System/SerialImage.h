#pragma once
#include"Game.h"

class SerialImage 
{
private:
	Texture m_texture;
	const Point m_size;
	const int m_maxSize;
	const int m_frame;
	volatile int m_index;
	volatile int m_count;
	volatile bool m_loop;
	volatile bool m_isEnd;
public:

//	template<class Type>
	SerialImage::SerialImage(const Texture& texture, const Point size, int maxSize, int frame, bool loop=true) :
		m_texture(texture),
		m_size(size),
		m_frame(frame),
		m_index(0),
		m_count(0),
		m_maxSize(maxSize),
		m_loop(loop),
		m_isEnd(false) {}

//	template<class Type>
	SerialImage::SerialImage(const Texture& texture, const Point size, int frame, bool loop=true) :
		SerialImage(texture, size, size.x*size.y, frame, loop) {}

	void update();
	const TexturedQuad rotate(double angle)const;
	const TextureRegion scale(double scale)const;
	const bool isEnd()const { return m_isEnd; }
	void setLoop(bool loop) { m_loop = loop; }
	void draw(const Vec2& pos,const Color& color=Palette::White) const;
	void drawAt(const Vec2& pos,const Color& color = Palette::White)const;
};
