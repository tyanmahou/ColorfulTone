#include <utils/Shader/Mask/MaskShader.hpp>

#include <commons/Constants.hpp>
#include <Siv3D.hpp>

namespace
{
	struct ShaderParam
	{
		s3d::int32 isEqual;
		Float3 _unused;
	};
}
namespace ct
{
	class MaskShader::Impl
	{
	public:
		Impl(uint32 width, uint32 height, size_t buffer) :
			m_maskPs(PixelShader::HLSL(Resource(U"Shaders/mask.ps")))
		{
			m_rt.reserve(buffer);
			for (size_t count = 0; count < buffer; ++count) {
				m_rt.emplace_back(width, height);
			}
			m_rt2.reserve(buffer);
			for (size_t count = 0; count < buffer; ++count) {
				m_rt2.emplace_back(width, height);
			}
		}

		void draw(MaskFunc func, const std::function<void()> mask)
		{
			m_rt[m_bufferIndex].clear(ColorF(0.0, 0.0));
			{
				static constexpr BlendState blend{
					true,
					Blend::Zero,
					Blend::Zero,
					BlendOp::Add,
					Blend::One,
					Blend::One,
				};
				ScopedRenderStates2D state(blend);
				ScopedRenderTarget2D target(m_rt[m_bufferIndex]);
				mask();
			}
			Graphics2D::SetPSTexture(1, m_rt[m_bufferIndex]);

			m_cb->isEqual = func == MaskFunc::Equal ? 1 : 0;
			Graphics2D::SetConstantBuffer(ShaderStage::Pixel, 1, m_cb);
			{
				static constexpr BlendState blend{
					true,
					Blend::One,
					Blend::SrcAlpha
				};
				ScopedRenderStates2D state(blend);
				ScopedCustomShader2D shader(m_maskPs);

				Transformer2D transLocal(Mat3x2::Identity(), Transformer2D::Target::SetLocal);
				Transformer2D transCamera(Mat3x2::Identity(), Transformer2D::Target::SetCamera);

				auto colorAdd = s3d::Graphics2D::GetColorAdd();
				auto colorMul = s3d::Graphics2D::GetColorMul();
				s3d::Graphics2D::Internal::SetColorMul(Float4(1, 1, 1, 1));
				s3d::Graphics2D::Internal::SetColorAdd(Float4(0, 0, 0, 0));
				m_rt2[m_bufferIndex].draw();
				s3d::Graphics2D::Internal::SetColorAdd(colorAdd);
				s3d::Graphics2D::Internal::SetColorMul(colorMul);
			}
		}

		const RenderTexture& getDrawerTarget()
		{
			return m_rt2[m_bufferIndex];
		}
		void useBuffer(size_t bufferIndex)
		{
			m_bufferIndex = bufferIndex;
		}
	private:
		PixelShader m_maskPs;
		ConstantBuffer<ShaderParam> m_cb;
		Array<RenderTexture> m_rt;
		Array<RenderTexture> m_rt2;
		size_t m_bufferIndex = 0;
	};
	MaskShader::MaskShader(const Size& size, size_t buffer) :
		MaskShader(size.x, size.y, buffer)
	{}
	MaskShader::MaskShader(uint32 width, uint32 height, size_t buffer) :
		m_pImpl(std::make_shared<Impl>(width, height, buffer))
	{}
	const MaskShader& MaskShader::useBuffer(size_t bufferIndex) const
	{
		m_pImpl->useBuffer(bufferIndex);
		return *this;
	}
	MaskShader::ScopedMask MaskShader::equal(const std::function<void()>& mask) const
	{
		return ScopedMask(this->m_pImpl, MaskFunc::Equal, mask);
	}
	MaskShader::ScopedMask MaskShader::notEqual(const std::function<void()>& mask) const
	{
		return ScopedMask(this->m_pImpl, MaskFunc::NotEqual, mask);
	}

	MaskShader::ScopedMask::ScopedMask(
		const std::shared_ptr<Impl>& parentImpl,
		MaskFunc func,
		const std::function<void()>& mask
	) :
		m_parentImpl(parentImpl),
		m_func(func),
		m_mask(mask),
		m_oldBlend(Graphics2D::GetBlendState()),
		m_oldTarget(Graphics2D::GetRenderTarget()),
		m_oldViewport(Graphics2D::GetViewport())
	{
		static BlendState blend{ true, Blend::SrcAlpha, Blend::InvSrcAlpha, BlendOp::Add, Blend::Zero, Blend::InvSrcAlpha };
		Graphics2D::Internal::SetBlendState(blend);
		const auto& rt = m_parentImpl->getDrawerTarget();
		rt.clear(s3d::Palette::Black);
		Graphics2D::Internal::SetRenderTarget(rt);
		Graphics2D::Internal::SetViewport(Rect(rt.size()));
	}
	MaskShader::ScopedMask::~ScopedMask()
	{
		Graphics2D::Internal::SetBlendState(m_oldBlend);
		Graphics2D::Internal::SetRenderTarget(m_oldTarget);
		Graphics2D::Internal::SetViewport(m_oldViewport);

		m_parentImpl->draw(m_func, m_mask);
	}
}
