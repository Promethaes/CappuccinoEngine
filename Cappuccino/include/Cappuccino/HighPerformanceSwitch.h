#ifndef HIGH_PERFORMANCE_SWITCH_H
#define HIGH_PERFORMANCE_SWITCH_H


// This piece of code was given to us by Shawn Matthews
// Auto-magically changes OpenGL to use the high performance GPU to render rather than the iGPU
// (for dual GPU systems)
extern "C" {
	inline __declspec(dllexport) unsigned long NvOptimusEnablement = 0x01;
	inline __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x01;
}



#endif