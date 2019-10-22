#pragma once

namespace Cappuccino {
	class Math {
	public:
		//add more math functions in here when needed

		template<class T>
		static T lerp(const T& start, const T& end, const float u);

		static float invLerp(const float want, const float start, const float end);
	};
	template<class T>
	inline T Math::lerp(const T& start, const T& end, const float u)
	{
		return (((1-u)*start) + (u*end));
	}
}