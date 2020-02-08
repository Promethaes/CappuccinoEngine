#pragma once

#include <iostream>

#define CAPP_PATH									std::string(std::getenv("CappuccinoPath")) + "\\Cappuccino\\"

#if _DEBUG || DEBUG

#include <glad/glad.h>
#include <windows.h>


#define RED		(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define YELLOW	(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)

#define CAPP_ASSERT(exp, ...) \
	if(!(exp)) {\
		CAPP_PRINT_ERROR("ERROR ASSERTING \"%s\"!", #exp);\
		CAPP_PRINT_ERROR(##__VA_ARGS__);\
		CAPP_PRINT_ERROR(__FUNCSIG__);\
		CAPP_PRINT_ERROR("[%s : %i]", __FILE__, __LINE__);\
		__debugbreak();\
	}

#define CAPP_GL_CALL(glCall)\
	while(glGetError() != GL_NO_ERROR);\
	glCall;\
	while(GLenum error = glGetError()) {\
		char* type = NULL;\
		if(error == GL_INVALID_ENUM)					type = "GL_INVALID_ENUM";\
		if(error == GL_INVALID_VALUE)					type = "GL_INVALID_VALUE";\
		if(error == GL_INVALID_OPERATION)				type = "GL_INVALID_OPERATION";\
		if(error == GL_INVALID_FRAMEBUFFER_OPERATION)	type = "GL_INVALID_FRAMEBUFFER_OPERATION";\
		if(error == GL_OUT_OF_MEMORY)					type = "GL_OUT_OF_MEMORY";\
		\
		CAPP_PRINT_WARNING("OPENGL ERROR: %s", type);\
		CAPP_PRINT_WARNING(__FUNCSIG__);\
		CAPP_PRINT_WARNING("[%s : %i ]", __FILE__, __LINE__);\
		__debugbreak();\
	}

#define CAPP_PRINT(message, ...) \
	printf(message, ##__VA_ARGS__)

#define CAPP_PRINT_N(message, ...) \
	printf(message, ##__VA_ARGS__);\
	printf("\n")

#define CAPP_PRINT_ERROR(message, ...) \
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);\
	printf(message, ##__VA_ARGS__);\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);\
	printf("\n")

#define CAPP_PRINT_WARNING(message, ...) \
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);\
	printf(message, ##__VA_ARGS__);\
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);\
	printf("\n")

#define SYS_EXIT(code) \
	system("pause");\
	exit(code)

#else

#define CAPP_ASSERT									__noop
#define CAPP_GL_CALL(glCall)						glCall
#define CAPP_PRINT									__noop
#define CAPP_PRINT_N								__noop
#define CAPP_PRINT_ERROR							__noop
#define CAPP_PRINT_WARNING							__noop
#define SYS_EXIT									__noop

#endif