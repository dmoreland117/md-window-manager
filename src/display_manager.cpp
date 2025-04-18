#include "display_manager.h"
#include "window_manager.h"

DisplayManager::DisplayManager() :
    m_display(nullptr),
    m_screen(0),
    m_screenWidth(0),
    m_screenHeight(0),
    m_screenDepth(0),
    m_logger(Logger::getLogger())
{
}

DisplayManager::~DisplayManager()
{
}

void DisplayManager::init()
{
    m_display = XOpenDisplay(nullptr);
    if (m_display == nullptr)
    {
        throw std::runtime_error("Failed to open display");
    }
    
    m_rootWindow = RootWindow(m_display, m_screen);
    if (m_rootWindow == 0)
    {
        m_logger.logError("Failed to get root window");
        throw std::runtime_error("Failed to get root window");
    }
    
    m_logger.loginfo("Root Window ID: " + std::to_string(m_rootWindow));
    m_logger.loginfo("root window depth: " + std::to_string(DefaultDepth(m_display, m_screen)));

    m_screen = DefaultScreen(m_display);
    m_screenWidth = DisplayWidth(m_display, m_screen);
    m_screenHeight = DisplayHeight(m_display, m_screen);
    m_screenDepth = DefaultDepth(m_display, m_screen);

    XSetErrorHandler(errorHandle);

    XSelectInput(m_display, m_rootWindow, SubstructureNotifyMask | SubstructureRedirectMask);

    XCompositeRedirectSubwindows(m_display, m_rootWindow, CompositeRedirectManual);
    XFlush(m_display);
}

void DisplayManager::destroy()
{
    if (m_display != nullptr)
    {
        XCloseDisplay(m_display);
        m_display = nullptr;
    }
    m_logger.loginfo("DisplayManager destroyed");
}

Display *DisplayManager::getDisplay() const
{
    return m_display;
}

Window DisplayManager::getRootWindow() const
{
    return m_rootWindow;
}

int DisplayManager::getScreen() const
{
    return m_screen;
}

unsigned int DisplayManager::getScreenWidth() const
{
    return m_screenWidth;
}

unsigned int DisplayManager::getScreenHeight() const
{
    return m_screenHeight;
}

unsigned int DisplayManager::getScreenDepth() const
{
    return m_screenDepth;
}

int DisplayManager::errorHandle(Display *display, XErrorEvent *error_event)
{
    Logger l = Logger::getLogger();

    l.logXError(error_event->request_code, error_event->error_code, error_event->resourceid, error_event->minor_code);

    return 0;
}
