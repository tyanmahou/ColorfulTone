#pragma once
#include<memory>
#include<Siv3D/Fwd.hpp>
#include<Siv3D/Color.hpp>

class SerialImage
{
private:
	class Impl;
	std::shared_ptr<Impl> m_pImpl;
public:

	SerialImage(
		const s3d::Texture& texture,
		const s3d::Point size,
		int maxSize,
		int frame,
		bool loop = true
	);

	SerialImage(
		const s3d::Texture& texture,
		const s3d::Point size,
		int frame,
		bool loop = true
	);

	void update()const;
	const s3d::TexturedQuad rotate(double angle)const;
	const s3d::TextureRegion scale(double scale)const;
	const bool isEnd()const;
	void setLoop(bool loop);
	void draw(const s3d::Vec2& pos, const s3d::Color& color = s3d::Palette::White) const;
	void drawAt(const s3d::Vec2& pos, const s3d::Color& color = s3d::Palette::White)const;
};
