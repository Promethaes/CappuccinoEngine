#pragma once

namespace Cappuccino {
	class Math {
	public:
		//add more math functions in here when needed

		template<class T>
		static T lerp(const T& start, const T& end, const float u);

		static float invLerp(const float want, const float start, const float end);
	};
}