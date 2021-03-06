#pragma once

#include <glfw/glfw3.h>

namespace Cappuccino {
	
	enum class KeyEvent {
		TILDE = GLFW_KEY_GRAVE_ACCENT,
		ONE = GLFW_KEY_1,
		TWO = GLFW_KEY_2,
		THREE = GLFW_KEY_3,
		FOUR = GLFW_KEY_4,
		FIVE = GLFW_KEY_5,
		SIX = GLFW_KEY_6,
		SEVEN = GLFW_KEY_7,
		EIGHT = GLFW_KEY_8,
		NINE = GLFW_KEY_9,
		ZERO = GLFW_KEY_0,
		MINUS = GLFW_KEY_MINUS,
		EQUAL = GLFW_KEY_EQUAL,
		BACKSPACE = GLFW_KEY_BACKSPACE,
		
		TAB = GLFW_KEY_TAB,
		LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
		RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
		ENTER = GLFW_KEY_ENTER,
		
		CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
		SEMICOLON = GLFW_KEY_SEMICOLON,
		APOSTROPHE = GLFW_KEY_APOSTROPHE,
		BACKSLASH = GLFW_KEY_BACKSLASH,
		
		LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
		COMMA = GLFW_KEY_COMMA,
		PERIOD = GLFW_KEY_PERIOD,
		SLASH = GLFW_KEY_SLASH,
		RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,

		LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
		LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
		LEFT_ALT = GLFW_KEY_LEFT_ALT,
		SPACE = GLFW_KEY_SPACE,
		RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
		RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,
		MENU = GLFW_KEY_MENU,
		RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,

		ESCAPE = GLFW_KEY_ESCAPE,
		F1 = GLFW_KEY_F1,
		F2 = GLFW_KEY_F2,
		F3 = GLFW_KEY_F3,
		F4 = GLFW_KEY_F4,
		F5 = GLFW_KEY_F5,
		F6 = GLFW_KEY_F6,
		F7 = GLFW_KEY_F7,
		F8 = GLFW_KEY_F8,
		F9 = GLFW_KEY_F9,
		F10 = GLFW_KEY_F10,
		F11 = GLFW_KEY_F11,
		F12 = GLFW_KEY_F12,
		F13 = GLFW_KEY_F13,
		F14 = GLFW_KEY_F14,
		F15 = GLFW_KEY_F15,
		F16 = GLFW_KEY_F16,
		F17 = GLFW_KEY_F17,
		F18 = GLFW_KEY_F18,
		F19 = GLFW_KEY_F19,
		F20 = GLFW_KEY_F20,
		F21 = GLFW_KEY_F21,
		F22 = GLFW_KEY_F22,
		F23 = GLFW_KEY_F23,
		F24 = GLFW_KEY_F24,
		F25 = GLFW_KEY_F25,
		
		HOME = GLFW_KEY_HOME,
		END = GLFW_KEY_END,
		PAGE_UP = GLFW_KEY_PAGE_UP,
		PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
		INSERT = GLFW_KEY_INSERT,
		DEL = GLFW_KEY_DELETE,
		
		PAUSE = GLFW_KEY_PAUSE,
		PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
		LEFT_ARROW = GLFW_KEY_LEFT,
		UP_ARROW = GLFW_KEY_UP,
		RIGHT_ARROW = GLFW_KEY_RIGHT,
		DOWN_ARROW = GLFW_KEY_DOWN,
		
		A = GLFW_KEY_A,
		B = GLFW_KEY_B,
		C = GLFW_KEY_C,
		D = GLFW_KEY_D,
		E = GLFW_KEY_E,
		F = GLFW_KEY_F,
		G = GLFW_KEY_G,
		H = GLFW_KEY_H,
		I = GLFW_KEY_I,
		J = GLFW_KEY_J,
		K = GLFW_KEY_K,
		L = GLFW_KEY_L,
		M = GLFW_KEY_M,
		N = GLFW_KEY_N,
		O = GLFW_KEY_O,
		P = GLFW_KEY_P,
		Q = GLFW_KEY_Q,
		R = GLFW_KEY_R,
		S = GLFW_KEY_S,
		T = GLFW_KEY_T,
		U = GLFW_KEY_U,
		V = GLFW_KEY_V,
		W = GLFW_KEY_W,
		X = GLFW_KEY_X,
		Y = GLFW_KEY_Y,
		Z = GLFW_KEY_Z,
		
		KP_0 = GLFW_KEY_KP_0,
		KP_1 = GLFW_KEY_KP_1,
		KP_2 = GLFW_KEY_KP_2,
		KP_3 = GLFW_KEY_KP_3,
		KP_4 = GLFW_KEY_KP_4,
		KP_5 = GLFW_KEY_KP_5,
		KP_6 = GLFW_KEY_KP_6,
		KP_7 = GLFW_KEY_KP_7,
		KP_8 = GLFW_KEY_KP_8,
		KP_9 = GLFW_KEY_KP_9,
		KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
		KP_ADD = GLFW_KEY_KP_ADD,
		KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
		KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
		KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
		
		NUM_LOCK = GLFW_KEY_NUM_LOCK,
		SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK
	};
	
}