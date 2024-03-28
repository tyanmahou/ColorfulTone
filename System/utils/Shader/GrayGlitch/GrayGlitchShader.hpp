#pragma once
#include <memory>
#include <Siv3D/ScopedCustomShader2D.hpp>

namespace ct
{
	class GrayGlitchShader
	{
		class Impl;
	public:
		GrayGlitchShader();
		~GrayGlitchShader();

		[[nodiscard]] s3d::ScopedCustomShader2D start() const;
	private:
		std::unique_ptr<Impl> m_pImpl;
	};
}