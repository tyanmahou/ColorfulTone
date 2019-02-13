#include"Object.h"
#include"Game.h"
#include"GameConfig.h"
bool Object::CanDraw(const Vec2 & pos)
{
	const double scale = Game::Config().m_playScale;
	if (pos.x <350.0 - 400.0 / scale || pos.x>450.0 + 400 / scale || pos.y <250 - 300.0 / scale || pos.y>350.0 + 300.0 / scale)
	{
		return false;
	}
	return true;
}
