#ifndef CAP_MACROS_H
#define CAP_MACROS_H

#include <windows.h>
#include <iostream>

#if _DEBUG

#define CAP_PRINT(message, ...)			printf(message, ##__VA_ARGS__); printf("\n");
#define CAP_PRINT_ERROR(message, ...)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0C);\
										printf(message, ##__VA_ARGS__);	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07); printf("\n");

#define SYS_EXIT(code)					system("pause"); exit(code);

#endif

#endif
