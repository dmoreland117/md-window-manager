#pragma once

#include <X11/Xlib.h>
#include <X11/extensions/Xcomposite.h>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <unistd.h>
#include "logger.h"
#include "types.h"
#include "util.h"
#include "display_manager.h"
#include "frame.h"

/// @brief This class initializes the display and runs the event loop.
class WindowManager {
public:
    WindowManager();
    ~WindowManager() {}

public: // init, destroy
    void init();
    void destroy();

public: // run
    void run();
    void stop();
    void setRunning(bool running);
    bool isRunning() const;

private:
    bool createRootPixmap();
    void destroyRootPixmap();

    bool createRootGC();
    void destroyRootGC();

    void handleEvents();
    void handleMapRequest(XEvent& event, WindowInfo windowInfo);
    void handleMapNotify(XEvent& event, WindowInfo windowInfo);
    void handleUnmapRequest(XEvent& event, WindowInfo windowInfo);
    void handleUnmapNotify(XEvent& event, WindowInfo windowInfo);
    void handleConfigureRequest(XEvent& event);
    void handleConfigureNotify(XEvent& event, WindowInfo windowInfo);
    void handleClientMessage(XEvent& event);
    void handleProportyNotify(XEvent& event, WindowInfo windowInfo);
    void handleCreateNotify(XEvent& event);
    void handleDestroyNotify(XEvent& event);
public:
    bool addDebugFrame(DColor backgroundColor, DColor borderColor, int borderWidth, std::string title);
    bool mapDebugFrame();
    void unmapDebugFrame();
    void DestroyDebugFrame(WindowInfo windowInfo);

    bool addDesktopFrame(WindowInfo windowInfo);
    bool mapDesktopFrame();
    void unmapDesktopFrame();
    void DestroyDesktopFrame(WindowInfo windowInfo);

    bool addLauncherFrame(WindowInfo windowInfo);
    bool mapLauncherFrame();
    void unmapLauncherFrame();
    void DestroyLauncherFrame(WindowInfo windowInfo);

    bool addDockFrame(WindowInfo windowInfo);
    bool mapDockFrame();
    void unmapDockFrame();
    void DestroyDockFrame(WindowInfo windowInfo);

    bool addFrame(WindowInfo windowInfo);
    bool mapFrame(int id);
    void unmapFrame(int id);
    void DestroyFrame(int id);

    bool addPopupMenuFrame(WindowInfo windowInfo);
    bool mapPopupMenuFrame(int id);
    void unmapPopupMenuFrame(int id);
    void DestroyPopupMenuFrame(int id);

private:
    DisplayManager* m_displayManagerPtr;
    Pixmap m_rootPixmap;
    GC m_rootGC;

    XEvent m_event;

    bool m_running;
    int m_desktopWinID, m_launcherWinID, m_dockWinID;
    
    Logger m_logger;
    std::map<Window, Frame> m_openWindows;
    std::map<Window, Frame> m_popupWindows;

    std::vector<Window> m_notRedirectedFrames;

};