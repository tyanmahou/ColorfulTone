#include"FontKinetic.h"

void FontKinetic::DeleteSpace(s3d::KineticTypography& k)
{
	{
		if (k.ch == ' ')
			k.col.setAlpha(0);
	}
}
