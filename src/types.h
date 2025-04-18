#pragma once

#include <X11/Xlib.h>
#include <string>

#define MAP_STATUS_UNMAPED 0
#define MAP_STATUS_UNVIEWABLE 1
#define MAP_STATUS_MAPED 2

typedef struct {
    Window windowPtr;
    Window parentWindowPtr;
    Window childWindowPtr;
    Window rootWindowPtr;
    std::string title;
    int posx;
    int posy;
    int width;
    int height;
    int mapStatus;
    int depth;
    std::string windowType;
    bool overrideRedirect;
    bool redirected;
    Visual* visual;
} WindowInfo;

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    unsigned long toXColor() const {
        return (red << 16) | (green << 8) | blue;
    }
    void fromXColor(unsigned long color) {
        red = (color >> 16) & 0xFF;
        green = (color >> 8) & 0xFF;
        blue = color & 0xFF;
    }
} DColor;
