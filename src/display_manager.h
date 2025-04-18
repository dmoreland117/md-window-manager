#pragma once

#include <X11/Xlib.h>
#include <X11/extensions/Xcomposite.h>
#include "logger.h"

class WindowManager;

class DisplayManager 
{
public:
    DisplayManager();
    ~DisplayManager();

public: // init, destroy
    void init();
    void destroy();

public: // geters and setters
    Display* getDisplay() const;
    Window getRootWindow() const;
    int getScreen() const;
    unsigned int getScreenWidth() const;
    unsigned int getScreenHeight() const;
    unsigned int getScreenDepth() const;

    static int errorHandle(Display *display, XErrorEvent *error_event);


private:
    Display* m_display;
    Window m_rootWindow;
    int m_screen;
    unsigned int m_screenWidth;
    unsigned int m_screenHeight;
    unsigned int m_screenDepth;

    Logger m_logger;
};