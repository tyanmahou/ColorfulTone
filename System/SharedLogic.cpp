#include "SharedLogic.hpp"
#include "Util.h"
#include "PlayKey.h"

namespace SharedLogic
{
	int MoveSelect()
	{
		if (util::AccelPressed(PlayKey::Down()))
		{
			return -1;
		}
		if (util::AccelPressed(PlayKey::Up()))
		{
			return 1;
		}
		return 0;
	}
}