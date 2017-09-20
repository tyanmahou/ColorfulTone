#include"Object.h"
#include"Game.h"
bool Object::CanDraw(const Vec2 & pos)
{
	const double scale = Game::Instance()->m_config.m_playScale;
		if (pos.x <350.0-400.0/scale || pos.x>450.0+400/scale  || pos.y <250-300.0 / scale || pos.y>350.0 +300.0 / scale)
		{
			return false;
		}
		//if (pos.x <-50.0 || pos.x>850.0 || pos.y <-50.0 || pos.y>650.0)
		//{
		//	return false;
		//}

		return true;
}
