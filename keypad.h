#include "gba.h" // for REG_P1, the keypad register

#define KEY_A      1<<0
#define KEY_B      1<<1
#define KEY_SELECT 1<<2
#define KEY_START  1<<3
#define KEY_RIGHT  1<<4
#define KEY_LEFT   1<<5
#define KEY_UP     1<<6
#define KEY_DOWN   1<<7
#define KEY_R      1<<8
#define KEY_L      1<<9

// bits are *cleared* when a button is pressed, invert so it's more intuitive 
#define KEYS ~REG_P1

#define isPressedA()      (KEYS & KEY_A)
#define isPressedB()      (KEYS & KEY_B)
#define isPressedSelect() (KEYS & KEY_SELECT)
#define isPressedStart()  (KEYS & KEY_START)
#define isPressedRight()  (KEYS & KEY_RIGHT)
#define isPressedLeft()   (KEYS & KEY_LEFT)
#define isPressedUp()     (KEYS & KEY_UP)
#define isPressedDown()   (KEYS & KEY_DOWN)
#define isPressedR()      (KEYS & KEY_R)
#define isPressedL()      (KEYS & KEY_L)
