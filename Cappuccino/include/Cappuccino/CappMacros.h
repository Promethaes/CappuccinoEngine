#pragma once

#include "Cappuccino/HighPerformanceSwitch.h"

#include <windows.h>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS

#define CAPP_PATH						std::string(std::getenv("CappuccinoPath"))

#if _DEBUG

#define SOUNDTEST true
#define CUBETEST true
#define SCENETEST true
#define NETWORKTEST false

#define CAPP_PRINT(message, ...)		printf(message, ##__VA_ARGS__)

#define CAPP_PRINT_N(message, ...)		printf(message, ##__VA_ARGS__);\
										printf("\n")

#define CAPP_PRINT_ERROR(message, ...)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0C);\
										printf(message, ##__VA_ARGS__);\
										SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);\
										printf("\n")

#define SYS_EXIT(code)					system("pause"); exit(code)

#else

#define CAPP_PRINT						__noop
#define CAPP_PRINT_N					__noop
#define CAPP_PRINT_ERROR				__noop
#define SYS_EXIT						__noop

#endif