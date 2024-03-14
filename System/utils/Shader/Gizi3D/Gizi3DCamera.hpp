#pragma once
#include <memory>
#include <Siv3D/ScopedCustomShader2D.hpp>
#include <Siv3D/Vector2D.hpp>

namespace ct
{
	class Gizi3DCamera : private s3d::BasicCamera3D
	{
		class Impl;
	public:
		Gizi3DCamera(const s3d::Size& size);
		~Gizi3DCamera();

		[[nodiscard]] ScopedCustomShader2D start() const;

		[[nodiscard]] s3d::Vec2 toScreenPos(const s3d::Vec2& pos) const;
	private:
		std::unique_ptr<Impl> m_pImpl;
	};
}