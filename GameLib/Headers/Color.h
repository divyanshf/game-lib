#pragma once
#include <iostream>
#include <windows.h>

class Color {
public:
    Color(int desiredColor) {
        consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        color = desiredColor;
    }

    friend std::ostream& operator<<(std::ostream& ss, Color obj) {
        SetConsoleTextAttribute(obj.consoleHandle, obj.color);
        return ss;
    }
private:
    int color;
    HANDLE consoleHandle;
};
