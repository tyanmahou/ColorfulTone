#include <utils/Shader/Gizi3D/Gizi3DCamera.hpp>
#include <Siv3D.hpp>

namespace ct
{
	struct ShaderParam
	{
		Float2 size;
		Mat4x4 viewProj;
	};
	class Gizi3DCamera::Impl : private s3d::BasicCamera3D
	{
	public:
		Impl(const s3d::Size& size) :
			Impl(size, { 0,0 })
		{}
		Impl(const s3d::Size& size, const Vec2& pos) :
			Impl(size, s3d::BasicCamera3D::DefaultVerticalFOV, { pos.x , pos.y + 300, 100 }, { pos.x, pos.y, 10 })
		{}
		Impl(const s3d::Size& size, double fov, const Vec3& eye, const Vec3& focus) :
			s3d::BasicCamera3D(size, fov),
			m_vs(HLSL(Resource(U"Shaders/gizi3d.vs")))
		{
			this->setEyePos(eye);
			this->setFocusPos(focus);
		}
		Impl& setZ(double z)
		{
			m_eyePosition.y = z;
			m_isDirtyView = true;
			return *this;
		}
		Impl& setEyePos(const Vec2& pos)
		{
			m_eyePosition.x = pos.x;
			m_eyePosition.z = -pos.y;
			m_isDirtyView = true;
			return *this;
		}
		Impl& setEyePos(const Vec3& pos)
		{
			m_eyePosition.x = pos.x;
			m_eyePosition.z = -pos.y;
			m_eyePosition.y = pos.z;
			m_isDirtyView = true;
			return *this;
		}
		Impl& setFocusPos(const Vec2& pos)
		{
			m_focusPosition.x = pos.x;
			m_focusPosition.z = -pos.y;
			m_isDirtyView = true;
			return *this;
		}
		Impl& setFocusPos(const Vec3& pos)
		{
			m_focusPosition.x = pos.x;
			m_focusPosition.z = -pos.y;
			m_focusPosition.y = pos.z;
			m_isDirtyView = true;
			return *this;
		}
		Impl& setFov(double fov)
		{
			m_verticalFOV = fov;
			m_isDirtyProj = true;
			return *this;
		}
		[[nodiscard]] Vec2 focusPos() const
		{
			return Vec2{ m_focusPosition.x,-m_focusPosition.z };
		}
		[[nodiscard]] Vec2 eyePos() const
		{
			return Vec2{ m_eyePosition.x,-m_eyePosition.z };
		}
		[[nodiscard]] ScopedCustomShader2D start()
		{
			if (m_isDirtyProj || m_isDirtyView) {
				if (m_isDirtyProj) {
					updateProj();
				}
				if (m_isDirtyView) {
					Vec3 upVec{ 0,1,0 };
					if (m_eyePosition.xz() == m_focusPosition.xz()) {
						upVec = Vec3{ 0,0,1 };
					}
					m_upDirection = upVec;
					updateView();
				}
				updateViewProj();

				m_isDirtyProj = m_isDirtyView = false;
				m_cb->size = Float2(m_sceneSize.x, m_sceneSize.y);
				m_cb->viewProj = getViewProj().transposed();
			}
			Graphics2D::SetVSConstantBuffer(1, m_cb);
			return ScopedCustomShader2D{ m_vs };
		}
		[[nodiscard]] Vec2 toScreenPos(const Vec2& pos) const
		{
			Float3 ret = this->worldToScreenPoint({ pos.x, 0, -pos.y });
			return { ret.x, ret.y };
		}
	private:
		VertexShader m_vs;
		ConstantBuffer<ShaderParam> m_cb;
		bool m_isDirtyProj = true;
		bool m_isDirtyView = true;
	};
    Gizi3DCamera::Gizi3DCamera(const s3d::Size& size):
		m_pImpl(std::make_unique<Impl>(size, 40_deg, Vec3{ 400, 800, 350 }, Vec3{ 400, 300, 0 }))
    {
    }
	Gizi3DCamera::~Gizi3DCamera()
	{
	}
	ScopedCustomShader2D Gizi3DCamera::start() const
	{
		return m_pImpl->start();
	}
	s3d::Vec2 Gizi3DCamera::toScreenPos(const s3d::Vec2& pos) const
	{
		return m_pImpl->toScreenPos(pos);
	}
}
