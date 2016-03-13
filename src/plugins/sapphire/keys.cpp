#include "keys.hpp"

namespace Sapphire {

const std::array<const char * const, SAPPHIRE_NUM_KEYS> key_names = {{
    "KEY_ESCAPE",    
    "KEY_F1",   
    "KEY_F2",
    "KEY_F3",
    "KEY_F4",
    "KEY_F5",
    "KEY_F6",
    "KEY_F7",
    "KEY_F8",
    "KEY_F9",
    "KEY_F10",    
    "KEY_F11",
    "KEY_F12",
    "KEY_TILDE",
    "KEY_0", 
    "KEY_1", 
    "KEY_2", 
    "KEY_3", 
    "KEY_4", 
    "KEY_5", 
    "KEY_6", 
    "KEY_7", 
    "KEY_8", 
    "KEY_9", 
    "KEY_MINUS",
    "KEY_EQUALS",
    "KEY_BACKSPACE",
    "KEY_TAB",
    "KEY_A", 
    "KEY_B", 
    "KEY_C", 
    "KEY_D", 
    "KEY_E", 
    "KEY_F", 
    "KEY_G", 
    "KEY_H", 
    "KEY_I", 
    "KEY_J", 
    "KEY_K", 
    "KEY_L", 
    "KEY_M", 
    "KEY_N", 
    "KEY_O", 
    "KEY_P", 
    "KEY_Q", 
    "KEY_R", 
    "KEY_S", 
    "KEY_T", 
    "KEY_U", 
    "KEY_V", 
    "KEY_W", 
    "KEY_X", 
    "KEY_Y", 
    "KEY_Z", 
    "KEY_SHIFT",
    "KEY_RSHIFT",
    "KEY_CTRL",
    "KEY_RCTRL",
    "KEY_ALT",
    "KEY_LALT",
    "KEY_SPACE",
    "KEY_OPENBRACE",
    "KEY_CLOSEBRACE",
    "KEY_SEMICOLON",
    "KEY_APOSTROPHE",
    "KEY_COMMA",
    "KEY_PERIOD",
    "KEY_SLASH",
    "KEY_BACKSLASH",
    "KEY_ENTER",
    "KEY_RETURN", // This is not in 1.5
    "KEY_INSERT",
    "KEY_DELETE",
    "KEY_HOME",
    "KEY_END",
    "KEY_PAGEUP",
    "KEY_PAGEDOWN",
    "KEY_UP",
    "KEY_RIGHT",
    "KEY_DOWN",
    "KEY_LEFT",
    "KEY_NUM_0",
    "KEY_NUM_1",
    "KEY_NUM_2",
    "KEY_NUM_3",
    "KEY_NUM_4",
    "KEY_NUM_5",
    "KEY_NUM_6",
    "KEY_NUM_7",
    "KEY_NUM_8",
    "KEY_NUM_9",
    "KEY_CAPSLOCK",
    "KEY_NUMLOCK",
    "KEY_SCROLLOCK", // For compatibility with Sphere 1.x 
    "KEY_SCROLLLOCK",  
}};

bool AsciiToKey(char i, int &what){
    if(i>='0' && i<='9')
        what = (i - '0') + KEY_0;
    else if(i>='a' && i<= 'z')
        what = (i - 'a') + KEY_A;
    else if(i>='A' && i<= 'Z')
        what = (i - 'A') + KEY_A;
    else if(i=='-' || i=='_')
        what = KEY_MINUS;
    else if(i=='+' || i=='=')
        what = KEY_EQUALS;
    else if(i=='[' || i=='{')
        what = KEY_OPENBRACE;
    else if(i==']' || i=='}')
        what = KEY_CLOSEBRACE;
    else if(i==';' || i==':')
        what = KEY_SEMICOLON;
    else if(i=='\'' || i=='"')
        what = KEY_APOSTROPHE;
    else if(i==',' || i=='<')
        what = KEY_COMMA;
    else if(i=='.' || i=='>')
        what = KEY_PERIOD;
    else if(i=='/' || i=='?')
        what = KEY_BACKSLASH;
    else if(i=='\' || i=='|')
        what = KEY_SLASH;
    else
        return false;

    return true;
}

} // namespace Sapphire
