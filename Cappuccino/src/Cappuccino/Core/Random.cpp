#include "Cappuccino/Core/Random.h"

#include <chrono>
#include <random>

using namespace std::chrono;

int Cappuccino::randomInt(int low, int high) {
	const high_resolution_clock::time_point timeNow = high_resolution_clock::now();
	const high_resolution_clock::duration seed = timeNow.time_since_epoch();
	
	std::default_random_engine generator(seed.count());
	const std::uniform_int_distribution distribution(low, high);

	return distribution(generator);
}

float Cappuccino::randomFloat(float low, float high) {
	const high_resolution_clock::time_point timeNow = high_resolution_clock::now();
	const high_resolution_clock::duration seed = timeNow.time_since_epoch();

	std::default_random_engine generator(seed.count());
	const std::uniform_real_distribution<float> distribution(low, high);

	return distribution(generator);
}

double Cappuccino::randomDouble(double low, double high) {
	const high_resolution_clock::time_point timeNow = high_resolution_clock::now();
	const high_resolution_clock::duration seed = timeNow.time_since_epoch();

	std::default_random_engine generator(seed.count());
	const std::uniform_real_distribution<double> distribution(low, high);

	return distribution(generator);
}
