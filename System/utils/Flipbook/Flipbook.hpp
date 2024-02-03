#pragma once
#include <memory>
#include <Siv3D/Fwd.hpp>
#include <Siv3D/Color.hpp>
#include <Siv3D/Texture.hpp>
#include <utils/Fps/Fps.hpp>

namespace ct
{
	class Flipbook
	{
	public:

		Flipbook(
			const s3d::Texture& texture,
			const s3d::Point size,
			s3d::int32 maxSize,
			bool loop = true,
			const Fps& fps = 60_fps
		);

		Flipbook(
			const s3d::Texture& texture,
			const s3d::Point size,
			bool loop = true,
			const Fps& fps = 60_fps
		);

		void update(double dt = Scene::DeltaTime())const;
		const s3d::TexturedQuad rotate(double angle)const;
		const s3d::TextureRegion scale(double scale)const;
		const bool isEnd()const;
		void setLoop(bool loop);
		void draw(const s3d::Vec2& pos, const s3d::Color& color = s3d::Palette::White) const;
		void drawAt(const s3d::Vec2& pos, const s3d::Color& color = s3d::Palette::White)const;
	private:
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
	};
}
