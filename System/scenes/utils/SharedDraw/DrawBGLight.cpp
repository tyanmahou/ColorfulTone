#include <scenes/utils/SharedDraw/DrawBGLight.hpp>
#include <effects/BGLight.hpp>
#include <Siv3D.hpp>

namespace ct::SharedDraw
{
	DrawBGLight::DrawBGLight():
		m_timer(1/3.0)
	{
	}
	void DrawBGLight::update()
	{
		m_timer.update(Scene::DeltaTime()).each([this](double) {
			m_effect.add<BGLight>();
		});
	}

	void DrawBGLight::draw() const
	{
		m_effect.update();
	}
}