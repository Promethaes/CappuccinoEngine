#pragma once

#include <windows.h>
#include <iostream>

#define CAPP_PATH						std::string(std::getenv("CappuccinoPath")) + "\\Cappuccino\\"

#ifdef _DEBUG

#define SOUNDTEST false
#define CUBETEST false
#define SCENETEST false
#define NETWORKTEST false
#define CROSSHAIRTEST false
#define TEXTRENDERTEST false
#define UITEST false

#define CAPP_PRINT(message, ...)		printf(message, ##__VA_ARGS__)

#define CAPP_PRINT_N(message, ...)		printf(message, ##__VA_ARGS__);\
										printf("\n")

#define CAPP_PRINT_ERROR(message, ...)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0C);\
										printf(message, ##__VA_ARGS__);\
										SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);\
										printf("\n")

#define SYS_EXIT(code)					system("pause"); exit(code)

#else

#define SOUNDTEST __noop
#define CUBETEST __noop
#define SCENETEST __noop
#define NETWORKTEST __noop

#define CAPP_PRINT						__noop
#define CAPP_PRINT_N					__noop
#define CAPP_PRINT_ERROR				__noop
#define SYS_EXIT						__noop

#endif