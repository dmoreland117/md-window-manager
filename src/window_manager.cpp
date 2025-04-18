#include "window_manager.h"

WindowManager::WindowManager() : m_displayManagerPtr(nullptr),
                                 m_rootPixmap(0),
                                 m_running(false),
                                 m_logger(Logger::getLogger())
{
}

void WindowManager::init()
{
    m_displayManagerPtr = new DisplayManager();
    m_displayManagerPtr->init();

    if (!createRootPixmap())
    {
        m_logger.logError("Failed to create root pixmap");
        return;
    }

    if (!createRootGC())
    {
        m_logger.logError("Failed to create root GC");
        return;
    }

    XSetForeground(m_displayManagerPtr->getDisplay(), m_rootGC, WhitePixel(m_displayManagerPtr->getDisplay(), m_displayManagerPtr->getScreen()));
    XFillRectangle(m_displayManagerPtr->getDisplay(), m_rootPixmap, m_rootGC,
                   0, 0, m_displayManagerPtr->getScreenWidth(), m_displayManagerPtr->getScreenHeight());

    XCopyArea(m_displayManagerPtr->getDisplay(), m_rootPixmap,
              m_displayManagerPtr->getRootWindow(), m_rootGC,
              0, 0, m_displayManagerPtr->getScreenWidth(), m_displayManagerPtr->getScreenHeight(),
              0, 0);

    XFlush(m_displayManagerPtr->getDisplay());
    m_running = true;
    m_logger.loginfo("WindowManager initialized");
}

void WindowManager::destroy()
{
    destroyRootPixmap();
    m_displayManagerPtr->destroy();
    delete m_displayManagerPtr;
    m_displayManagerPtr = nullptr;

    m_logger.loginfo("WindowManager destroyed");
}

void WindowManager::run()
{

    while (m_running)
    {

        if (!m_running)
        {
            m_logger.logError("WindowManager stopped");
            return;
        }

        handleEvents();

        for (auto &frame : m_openWindows)
        {
            frame.second.update();
        }
        for (auto &frame : m_openWindows)
        {
            frame.second.blit(m_rootPixmap);
        }
        XCopyArea(m_displayManagerPtr->getDisplay(), m_rootPixmap,
                  m_displayManagerPtr->getRootWindow(), m_rootGC,
                  0, 0, m_displayManagerPtr->getScreenWidth(), m_displayManagerPtr->getScreenHeight(),
                  0, 0);
        XFlush(m_displayManagerPtr->getDisplay());
        usleep(1000); // Sleep for 1ms
    }
}

void WindowManager::stop()
{
    m_running = false;
}

void WindowManager::setRunning(bool running)
{
    m_running = running;
}

bool WindowManager::isRunning() const
{
    return m_running;
}

bool WindowManager::createRootPixmap()
{
    m_rootPixmap = XCreatePixmap(m_displayManagerPtr->getDisplay(),
                                 m_displayManagerPtr->getRootWindow(),
                                 m_displayManagerPtr->getScreenWidth(),
                                 m_displayManagerPtr->getScreenHeight(),
                                 m_displayManagerPtr->getScreenDepth());

    if (m_rootPixmap == 0)
    {
        m_logger.logError("Failed to create root pixmap");
        return false;
    }

    return true;
    XFlush(m_displayManagerPtr->getDisplay());
}

void WindowManager::destroyRootPixmap()
{
    if (m_rootPixmap != 0)
    {
        XFreePixmap(m_displayManagerPtr->getDisplay(), m_rootPixmap);
        m_rootPixmap = 0;
    }
    m_logger.loginfo("Root pixmap destroyed");
}

bool WindowManager::createRootGC()
{
    m_rootGC = XCreateGC(m_displayManagerPtr->getDisplay(),
                         m_displayManagerPtr->getRootWindow(),
                         0, nullptr);

    if (m_rootGC == 0)
    {
        m_logger.logError("Failed to create root GC");
        return false;
    }

    XFlush(m_displayManagerPtr->getDisplay());
    return true;
}

void WindowManager::destroyRootGC()
{
    if (m_rootGC != 0)
    {
        XFreeGC(m_displayManagerPtr->getDisplay(), m_rootGC);
        m_rootGC = 0;
    }
    m_logger.loginfo("Root GC destroyed");
}

void WindowManager::handleEvents()
{
    while (XPending(m_displayManagerPtr->getDisplay()) > 0)
    {
        XNextEvent(m_displayManagerPtr->getDisplay(), &m_event);

        WindowInfo windowInfo;
        switch (m_event.type)
        {
        case MapRequest:
            getWindowInfoFromWindow(m_displayManagerPtr->getDisplay(), m_event.xmaprequest.window, &windowInfo);
            handleMapRequest(m_event, windowInfo);
            break;
        case MapNotify:
            if (getWindowInfoFromWindow(m_displayManagerPtr->getDisplay(), m_event.xmap.window, &windowInfo))
                handleMapNotify(m_event, windowInfo);
            break;
        case UnmapNotify:
            if (getWindowInfoFromWindow(m_displayManagerPtr->getDisplay(), m_event.xunmap.window, &windowInfo))
                handleUnmapNotify(m_event, windowInfo);
            break;
        case ConfigureRequest:
            
            handleConfigureRequest(m_event);
            break;
        case ConfigureNotify:
            if (getWindowInfoFromWindow(m_displayManagerPtr->getDisplay(), m_event.xconfigure.window, &windowInfo))
                handleConfigureNotify(m_event, windowInfo);
            break;
        case ClientMessage:
            handleClientMessage(m_event);
            break;
        case PropertyNotify:
            if (getWindowInfoFromWindow(m_displayManagerPtr->getDisplay(), m_event.xproperty.window, &windowInfo))
                handleProportyNotify(m_event, windowInfo);
            break;
        case CreateNotify:
            handleCreateNotify(m_event);
            break;
        case DestroyNotify:
            handleDestroyNotify(m_event);
        default:

            if (m_event.type == 14)
                return;

            m_logger.logError("Unknown event type");
            m_logger.loginfo("   ID: " + std::to_string(m_event.xany.window));
            m_logger.loginfo("Event type: " + std::to_string(m_event.type));

            return;
            getWindowInfoFromWindow(m_displayManagerPtr->getDisplay(), m_event.xany.window, &windowInfo);
            m_logger.loginfo("Window ID: " + std::to_string(m_event.xany.window));
            m_logger.loginfo("Window title: " + windowInfo.title);
            m_logger.loginfo("Window type: " + windowInfo.windowType);

            break;
        }
    }
}

void WindowManager::handleMapRequest(XEvent &event, WindowInfo windowInfo)
{
    m_logger.loginfo("Handling MapRequest event");
    // Handle the MapRequest event here
    // You can use the windowInfo to get information about the window
    // For example, you can print the window title and position
    m_logger.loginfo("Window ID: " + std::to_string(windowInfo.windowPtr));
    m_logger.loginfo("    title: " + windowInfo.title);
    m_logger.loginfo("    type: " + windowInfo.windowType);
    m_logger.loginfo("    posx: " + std::to_string(windowInfo.posx));
    m_logger.loginfo("    posy: " + std::to_string(windowInfo.posy));
    m_logger.loginfo("    width: " + std::to_string(windowInfo.width));
    m_logger.loginfo("    height: " + std::to_string(windowInfo.height));
    m_logger.loginfo("    depth: " + std::to_string(windowInfo.depth));

    if (windowInfo.overrideRedirect)
    {
        m_logger.loginfo("Window is override redirect");
        m_logger.loginfo("Skipping mapping");
        return;
    }

    addFrame(windowInfo);

    m_notRedirectedFrames.erase(std::remove(m_notRedirectedFrames.begin(), m_notRedirectedFrames.end(), windowInfo.windowPtr), m_notRedirectedFrames.end());
    XMapWindow(m_displayManagerPtr->getDisplay(), windowInfo.windowPtr);
    m_logger.loginfo("Window mapped");
}

void WindowManager::handleMapNotify(XEvent &event, WindowInfo windowInfo)
{
    m_logger.loginfo("Handling MapNotify event");
    // Handle the MapNotify event here
    // You can use the windowInfo to get information about the window
    // For example, you can print the window title and position
    m_logger.loginfo("Window ID: " + std::to_string(windowInfo.windowPtr));

    m_logger.loginfo("Window title: " + windowInfo.title);

    XWindowAttributes attr;
    XGetWindowAttributes(m_displayManagerPtr->getDisplay(), windowInfo.windowPtr, &attr);

    while (attr.map_state != IsViewable)
    {
        XGetWindowAttributes(m_displayManagerPtr->getDisplay(), windowInfo.windowPtr, &attr);
    }

    m_logger.loginfo("window is vewable");

    XSelectInput(m_displayManagerPtr->getDisplay(), windowInfo.windowPtr, StructureNotifyMask | SubstructureNotifyMask | SubstructureRedirectMask | PropertyChangeMask | FocusChangeMask);

    if (windowInfo.overrideRedirect && m_popupWindows.find(windowInfo.windowPtr) != m_popupWindows.end())
    {
        m_logger.loginfo("Window is override redirect");
        m_logger.loginfo("probably popup menue");
        addPopupMenuFrame(windowInfo);
        mapPopupMenuFrame(windowInfo.windowPtr);
        m_popupWindows[windowInfo.windowPtr].setIsRedirected(true);

        m_notRedirectedFrames.erase(std::remove(m_notRedirectedFrames.begin(), m_notRedirectedFrames.end(), windowInfo.windowPtr), m_notRedirectedFrames.end());
        return;
    }

    mapFrame(windowInfo.windowPtr);
}

void WindowManager::handleUnmapRequest(XEvent &event, WindowInfo windowInfo)
{
    m_logger.loginfo("Handling UnmapRequest event");
    // Handle the UnmapRequest event here
    // You can use the windowInfo to get information about the window
    // For example, you can print the window title and position
    m_logger.loginfo("Window ID: " + std::to_string(windowInfo.windowPtr));

    m_logger.loginfo("Window title: " + windowInfo.title);
    m_logger.loginfo("Window type: " + windowInfo.windowType);

    XWindowAttributes attrs;
    XGetWindowAttributes(m_displayManagerPtr->getDisplay(), windowInfo.windowPtr, &attrs);

    if (attrs.override_redirect)
    {
        m_logger.loginfo("Window is override redirect");
        m_logger.loginfo("Skipping unmapping");
        return;
    }
    if (attrs.map_state == IsViewable)
    {
        unmapFrame(windowInfo.windowPtr);
        m_logger.loginfo("Window unmapped");
    }
}

void WindowManager::handleUnmapNotify(XEvent &event, WindowInfo windowInfo)
{
    m_logger.loginfo("Handling UnmapNotify event");
    // Handle the UnmapNotify event here
    // You can use the windowInfo to get information about the window
    // For example, you can print the window title and position
    m_logger.loginfo("Window ID: " + std::to_string(windowInfo.windowPtr));

    m_logger.loginfo("Window title: " + windowInfo.title);
}

void WindowManager::handleConfigureRequest(XEvent &event)
{
    m_logger.loginfo("Handling ConfigureRequest event");
    // Handle the ConfigureRequest event here
    // You can use the windowInfo to get information about the window
    // For example, you can print the window title and position
    m_logger.loginfo("Window ID: " + std::to_string(event.xconfigurerequest.window));

    XWindowAttributes attrs;
    XGetWindowAttributes(m_displayManagerPtr->getDisplay(), event.xconfigurerequest.window, &attrs);

    if (attrs.override_redirect)
    {
        m_logger.loginfo("Window is override redirect");
        m_logger.loginfo("Skipping config");
        return;
    }
    XConfigureRequestEvent &cr = event.xconfigurerequest;

    XWindowChanges changes;
    changes.x = 0;
    changes.y = 0;
    changes.width = m_displayManagerPtr->getScreenWidth();
    changes.height = m_displayManagerPtr->getScreenHeight();
    changes.border_width = cr.border_width;
    changes.sibling = cr.above;
    changes.stack_mode = cr.detail;

    m_logger.loginfo("Config Values:");
    m_logger.loginfo("x: " + std::to_string(cr.x));
    m_logger.loginfo("y: " + std::to_string(cr.y));
    m_logger.loginfo("width: " + std::to_string(cr.width));
    m_logger.loginfo("height: " + std::to_string(cr.height));
    m_logger.loginfo("border_width: " + std::to_string(cr.border_width));
    m_logger.loginfo("sibling: " + std::to_string(cr.above));
    m_logger.loginfo("stack_mode: " + std::to_string(cr.detail));

    XConfigureWindow(m_displayManagerPtr->getDisplay(), cr.window, cr.value_mask, &changes);
    XFlush(m_displayManagerPtr->getDisplay());
}

void WindowManager::handleConfigureNotify(XEvent &event, WindowInfo windowInfo)
{
    m_logger.loginfo("Handling ConfigureNotify event");
    // Handle the ConfigureNotify event here
    // You can use the windowInfo to get information about the window
    // For example, you can print the window title and position
    m_logger.loginfo("Window ID: " + std::to_string(windowInfo.windowPtr));

    m_logger.loginfo("Window title: " + windowInfo.title);
    if (windowInfo.overrideRedirect && m_popupWindows.find(windowInfo.windowPtr) != m_popupWindows.end())
    {
        m_logger.loginfo("Window is override redirect");

        Frame popupFrame = m_popupWindows[windowInfo.windowPtr];
        if (!popupFrame.getIsRedirected())
        {
            XCompositeRedirectWindow(m_displayManagerPtr->getDisplay(), windowInfo.windowPtr, CompositeRedirectManual);
            popupFrame.setIsRedirected(true);
        }
    }
}

void WindowManager::handleClientMessage(XEvent &event)
{
    m_logger.loginfo("handling clien message");
}

void WindowManager::handleProportyNotify(XEvent &event, WindowInfo windowInfo)
{
    Atom prop = event.xproperty.atom;
    char *propName = XGetAtomName(m_displayManagerPtr->getDisplay(), prop);
    std::cout << "[PROPERTY] Window: " << event.xproperty.window
              << " changed: " << (propName ? propName : "unknown") << "\n";
    if (propName)
        XFree(propName);
}

void WindowManager::handleCreateNotify(XEvent &event)
{
    m_logger.loginfo("Handling CreateNotify event");
    // Handle the CreateNotify event here
    // You can use the windowInfo to get information about the window
    // For example, you can print the window title and position
    m_logger.loginfo("Window ID: " + std::to_string(event.xcreatewindow.window));

    m_notRedirectedFrames.push_back(event.xcreatewindow.window);
}

void WindowManager::handleDestroyNotify(XEvent &event)
{
    if (m_openWindows.find(event.xdestroywindow.window) != m_openWindows.end())
    {
        m_logger.loginfo("Handling DestroyNotify event");
        // Handle the DestroyNotify event here
        // You can use the windowInfo to get information about the window
        // For example, you can print the window title and position
        m_logger.loginfo("Window ID: " + std::to_string(event.xdestroywindow.window));

        DestroyFrame(event.xdestroywindow.window);

        m_openWindows.erase(event.xdestroywindow.window);
        m_logger.loginfo("Window destroyed");
    }
}

bool WindowManager::addFrame(WindowInfo windowInfo)
{
    if (m_openWindows.find(windowInfo.windowPtr) != m_openWindows.end())
    {
        m_logger.loginfo("Window already exists");
        return false;
    }

    Frame frame(m_displayManagerPtr->getDisplay(), windowInfo);
    frame.setIsMapped(false);
    frame.setIsOverrideRedirect(windowInfo.overrideRedirect);
    frame.setIsRedirected(false);
    frame.setIsVisible(false);
    m_openWindows[windowInfo.windowPtr] = frame;

    m_logger.loginfo("Frame added");
    m_logger.loginfo("Current open windows: " + std::to_string(m_openWindows.size()));

    return true;
}

bool WindowManager::mapFrame(int id)
{
    auto it = m_openWindows.find(id);
    if (it == m_openWindows.end())
    {
        m_logger.loginfo("Frame not found");
        return false;
    }

    Frame &frame = it->second;
    frame.show();
    m_logger.loginfo("Frame mapped");
    m_logger.loginfo("Frame ID: " + std::to_string(id));
    usleep(1500);
    XFlush(m_displayManagerPtr->getDisplay());

    return true;
}

void WindowManager::unmapFrame(int id)
{
    auto it = m_openWindows.find(id);
    if (it == m_openWindows.end())
    {
        m_logger.loginfo("Frame not found");
        return;
    }

    Frame &frame = it->second;
    frame.hide();
    m_logger.loginfo("Frame unmapped");
}

void WindowManager::DestroyFrame(int id)
{
    auto it = m_openWindows.find(id);
    if (it == m_openWindows.end())
    {
        m_logger.loginfo("Frame not found");
        return;
    }

    Frame &frame = it->second;
    frame.hide();
    m_openWindows.erase(it);
    m_logger.loginfo("Frame destroyed");
    m_logger.loginfo("Frame ID: " + std::to_string(id));
}

bool WindowManager::addPopupMenuFrame(WindowInfo windowInfo)
{
    if (m_popupWindows.find(windowInfo.windowPtr) != m_popupWindows.end())
    {
        m_logger.loginfo("Popup menu already exists");
        return false;
    }

    Frame frame(m_displayManagerPtr->getDisplay(), windowInfo);
    m_popupWindows[windowInfo.windowPtr] = frame;

    m_logger.loginfo("Popup menu added");
    m_logger.loginfo("Current open popup menus: " + std::to_string(m_popupWindows.size()));

    return true;
}

bool WindowManager::mapPopupMenuFrame(int id)
{
    auto it = m_popupWindows.find(id);
    if (it == m_popupWindows.end())
    {
        m_logger.loginfo("Popup menu not found");
        return false;
    }

    Frame &frame = it->second;
    frame.show();
    m_logger.loginfo("Popup menu mapped");
    m_logger.loginfo("Popup menu ID: " + std::to_string(id));

    XFlush(m_displayManagerPtr->getDisplay());

    return true;
}
