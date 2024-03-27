#pragma once
#include <memory>
#include <Siv3D/ScopedCustomShader2D.hpp>

namespace ct
{
	class GrayscaleShader
	{
		class Impl;
	public:
		GrayscaleShader();
		~GrayscaleShader();

		[[nodiscard]] s3d::ScopedCustomShader2D start() const;
	private:
		std::unique_ptr<Impl> m_pImpl;
	};
}