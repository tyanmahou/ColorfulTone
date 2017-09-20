#include"FontKinetic.h"

void FontKinetic::DeleteSpace(KineticTypography& k)
{
	{
		if (k.ch == ' ')
			k.col.setAlpha(0);
	}
}
