#include "Cappuccino/CappMath.h"

namespace Cappuccino {
	float Math::invLerp(const float want, const float start, const float end)
	{
		return ((want - start)/(end - start));
	}
}