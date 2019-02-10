#include "SharedDraw.hpp"
#include "BGLight.hpp"
void SharedDraw::DrawBGLight::update()
{
	if (System::FrameCount() % 20 == 0)
	{
		m_effect.add<BGLight>();
	}
}

void SharedDraw::DrawBGLight::draw() const
{
	m_effect.update();
}
