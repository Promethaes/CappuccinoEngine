#include "Cappuccino/Random.h"

#include <chrono>
#include <random>

using namespace std::chrono;

bool Cappuccino::randomBool() {
	return static_cast<bool>(randomInt(0, 1));
}

int Cappuccino::randomInt(const int low, const int high) {
	const high_resolution_clock::time_point timeNow = high_resolution_clock::now();
	const high_resolution_clock::duration seed = timeNow.time_since_epoch();
	
	std::default_random_engine generator(seed.count());
	const std::uniform_int_distribution distribution(low, high);

	return distribution(generator);
}

float Cappuccino::randomFloat(const float low, const float high) {
	const high_resolution_clock::time_point timeNow = high_resolution_clock::now();
	const high_resolution_clock::duration seed = timeNow.time_since_epoch();

	std::default_random_engine generator(seed.count());
	const std::uniform_real_distribution<float> distribution(low, high);

	return distribution(generator);
}

double Cappuccino::randomDouble(const double low, const double high) {
	const high_resolution_clock::time_point timeNow = high_resolution_clock::now();
	const high_resolution_clock::duration seed = timeNow.time_since_epoch();

	std::default_random_engine generator(seed.count());
	const std::uniform_real_distribution<double> distribution(low, high);

	return distribution(generator);
}
