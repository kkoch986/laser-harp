#ifndef MENU_H
#define MENU_H

#include <Keypad.h>
#include <Key.h>

class Menu {
	public:
		static void init();
        static void start();
        static void clear();
        static void repaint();
        static void startCalibrating();
        static void handleKeyPress(KeypadEvent key);
        static void getKey();
};

#endif
