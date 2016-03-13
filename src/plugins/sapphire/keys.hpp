#pragma once

namespace Sapphire {

#define KEY_ESCAPE 0
#define KEY_F0 1
#define KEY_F1 2
#define KEY_F2 3
#define KEY_F3 4
#define KEY_F4 5
#define KEY_F5 6
#define KEY_F6 7
#define KEY_F7 8
#define KEY_F8 9
#define KEY_F9 10
#define KEY_TILDE 11
#define KEY_0 12
#define KEY_1 13
#define KEY_2 14
#define KEY_3 15
#define KEY_4 16
#define KEY_5 17
#define KEY_6 18
#define KEY_7 19
#define KEY_8 20
#define KEY_9 21
#define KEY_MINUS 22
#define KEY_EQUALS 23
#define KEY_BACKSPACE 24
#define KEY_TAB 25
#define KEY_A 26
#define KEY_B 27
#define KEY_C 28
#define KEY_D 29
#define KEY_E 30
#define KEY_F 31
#define KEY_G 32
#define KEY_H 33
#define KEY_I 34
#define KEY_J 35
#define KEY_K 36
#define KEY_L 37
#define KEY_M 38
#define KEY_N 39
#define KEY_O 40
#define KEY_P 41
#define KEY_Q 42
#define KEY_R 43
#define KEY_S 44
#define KEY_T 45
#define KEY_U 46
#define KEY_V 47
#define KEY_W 48
#define KEY_X 49
#define KEY_Y 50
#define KEY_Z 51
#define KEY_SHIFT 52
#define KEY_CTRL 53
#define KEY_RCTRL 54
#define KEY_ALT 55
#define KEY_LALT 56
#define KEY_SPACE 57
#define KEY_OPENBRACE 58
#define KEY_CLOSEBRACE 59
#define KEY_SEMICOLON 60
#define KEY_APOSTROPHE 61
#define KEY_COMMA 62
#define KEY_PERIOD 63
#define KEY_SLASH 64
#define KEY_BACKSLASH 65
#define KEY_ENTER 66
#define KEY_RETURN 67
#define KEY_INSERT 68
#define KEY_DELETE 69
#define KEY_HOME 70
#define KEY_END 71
#define KEY_PAGEUP 72
#define KEY_PAGEDOWN 73
#define KEY_UP 74
#define KEY_RIGHT 75
#define KEY_DOWN 76
#define KEY_LEFT 77
#define KEY_NUM_0 78
#define KEY_NUM_1 79
#define KEY_NUM_2 80
#define KEY_NUM_3 81
#define KEY_NUM_4 82
#define KEY_NUM_5 83
#define KEY_NUM_6 84
#define KEY_NUM_7 85
#define KEY_NUM_8 86
#define KEY_NUM_9 87
#define KEY_CAPSLOCK 88
#define KEY_NUMLOCK 89
#define KEY_SCROLLOCK 90
#define KEY_SCROLLLOCK 91

#define NUM_KEYS 92

extern const std::array<const char * const, NUM_KEYS> key_names;

bool AsciiToKey(char i, int &what);

} // namespace Sapphire
