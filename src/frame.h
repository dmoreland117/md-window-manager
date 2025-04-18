#include <X11/Xlib.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>
#include <string>
#include "types.h"
#include "logger.h"

class Frame {
public:
    Frame() = default;
    Frame(Display* display, WindowInfo window);
    Frame(Display* display, DColor backgroundColor);
    // TODO: Frame(Display* display, Pixmap pixmap)
    ~Frame();

    void setBackgroundColor(const DColor color);
    void setBorderColor(const DColor color);
    void setBorderWidth(int width);

    void setTitle(const std::string title);
    void setPosition(int x, int y);
    void setSize(unsigned int width, unsigned int height);

    void setIsMapped(bool mapped);
    void setIsOverrideRedirect(bool overrideRedirect);
    void setIsRedirected(bool redirected);

    void setIsVisible(bool visible);

    bool getIsMapped() const;
    bool getIsOverrideRedirect() const;
    bool getIsVisible() const;
    bool getIsRedirected() const;

    bool show();
    bool hide();

    bool isOverrideRedirect() const;

    void update();
    void blit(Pixmap pixmap);

private:
    void drawframeDecorations();
    void drawClientWindow();

    void createFrameDecorationPixmap();
    void createClientWindowPixmap();

    void createFrameDecorationGC();
    void createClientWindowGC();

private:
    WindowInfo m_windowInfo;
    
    Display* m_display;
    Window m_clientWindow;

    Pixmap m_frameDecorationPixmap;
    Pixmap m_clientWindowPixmap;

    GC m_frameDecorationGC;
    GC m_clientWindowGC; // not used

    DColor m_backgroundColor, m_borderColor;
Logger m_logger;

    int m_borderWidth;
    std::string m_title;
    int m_posx, m_posy;
    unsigned int m_width, m_height;
    bool m_mapped;
    bool m_overrideRedirect;
    bool m_visible;
    bool m_redirected;
};