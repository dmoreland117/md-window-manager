#include "frame.h"

Frame::Frame(Display *display, WindowInfo window) : m_display(display),
                                                    m_windowInfo(window)
{
    m_clientWindow = window.windowPtr;

    m_frameDecorationPixmap = 0;
    m_clientWindowPixmap = 0;
    m_frameDecorationGC = 0;
    m_clientWindowGC = 0;
    m_backgroundColor = {0, 25, 200}; // default color
    m_borderColor = {0, 0, 0};        // default color
    m_borderWidth = 2;                // default width
    m_title = window.title;
    m_posx = window.posx;
    m_posy = window.posy;
    m_width = window.width;
    m_height = window.height;
    m_mapped = false;
    m_overrideRedirect = false;
    m_logger = Logger::getLogger();
    m_visible = false;

    createFrameDecorationPixmap();
    createFrameDecorationGC();
}

Frame::Frame(Display *display, DColor backgroundColor)
{
}

Frame::~Frame()
{
}

void Frame::setBackgroundColor(const DColor color)
{
    m_backgroundColor = color;
}

void Frame::setBorderColor(const DColor color)
{
}

void Frame::setBorderWidth(int width)
{
}

void Frame::setPosition(int x, int y)
{
    m_posx = x;
    m_posy = y;
}

void Frame::setSize(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
}

void Frame::setIsMapped(bool mapped)
{
    m_mapped = mapped;
    if (mapped)
    {
        m_logger.loginfo("Frame is mapped");
    }
    else
    {
        m_logger.loginfo("Frame is unmapped");
    }
}

void Frame::setIsOverrideRedirect(bool overrideRedirect)
{
    m_overrideRedirect = overrideRedirect;
    if (overrideRedirect)
    {
        m_logger.loginfo("Frame is override redirect");
    }
    else
    {
        m_logger.loginfo("Frame is not override redirect");
    }
}

void Frame::setIsRedirected(bool redirected)
{
    m_redirected = redirected;
}

void Frame::setIsVisible(bool visible)
{
}

bool Frame::getIsMapped() const
{
    return false;
}

bool Frame::getIsOverrideRedirect() const
{
    return false;
}

bool Frame::getIsVisible() const
{
    return false;
}

bool Frame::getIsRedirected() const
{
    return false;
}

bool Frame::show()
{

    createClientWindowPixmap();

    m_visible = true;
    return true;
}

bool Frame::hide()
{

    m_visible = false;
    return true;
}

bool Frame::isOverrideRedirect() const
{
    return false;
}

void Frame::update()
{
    if (!m_visible)
    {
        //m_logger.loginfo("Frame is not visible");
        return;
    }
    drawframeDecorations();

    if (!m_clientWindow)
    {
        m_logger.logError("Client window is not set");
        return;
    }
    drawClientWindow();
}

void Frame::blit(Pixmap pixmap)
{
    XRenderPictFormat* srcFormat = XRenderFindVisualFormat(m_display, m_windowInfo.visual);
    XRenderPictFormat* dstFormat = XRenderFindStandardFormat(m_display, PictStandardRGB24);

    XRenderPictureAttributes pa;
    pa.subwindow_mode = IncludeInferiors;

    Picture srcPic = XRenderCreatePicture(
        m_display,
        m_frameDecorationPixmap,
        srcFormat,
        CPSubwindowMode,
        &pa);
    Picture dstPic = XRenderCreatePicture(
        m_display,
        pixmap,
        dstFormat,
        CPSubwindowMode,
        &pa);
    XRenderComposite(
        m_display,
        PictOpOver,
        srcPic,
        None,
        dstPic,
        0, 0,
        0, 0,
        m_posx, m_posy,
        m_width, m_height);
    XRenderFreePicture(m_display, srcPic);
    XRenderFreePicture(m_display, dstPic);
}

void Frame::drawframeDecorations()
{
    XSetForeground(m_display, m_frameDecorationGC, 0xff0000);
    XFillRectangle(
        m_display,
        m_frameDecorationPixmap,
        m_frameDecorationGC,
        0, 0,
        m_width, m_height);
}

void Frame::drawClientWindow()
{

    XRenderPictFormat *format = XRenderFindVisualFormat(m_display, m_windowInfo.visual);

    XRenderPictureAttributes pa;
    pa.subwindow_mode = IncludeInferiors;

    Picture srcPic = XRenderCreatePicture(
        m_display,
        m_clientWindowPixmap,
        format,
        CPSubwindowMode,
        &pa);

    Picture dstPic = XRenderCreatePicture(
        m_display,
        m_frameDecorationPixmap,
        format,
        CPSubwindowMode,
        &pa);

    XRenderComposite(
        m_display,
        PictOpOver,
        srcPic,
        None,
        dstPic,
        0, 0,
        0, 0,
        0, 0,
        m_width, m_height);

    XRenderFreePicture(m_display, srcPic);
    XRenderFreePicture(m_display, dstPic);
}

void Frame::createFrameDecorationPixmap()
{
    m_logger.loginfo("Creating frame decoration pixmap");

    m_frameDecorationPixmap = XCreatePixmap(
        m_display,
        DefaultRootWindow(m_display),
        m_width, m_height,
        m_windowInfo.depth);
    if (m_frameDecorationPixmap == 0)
    {
    }
}

void Frame::createClientWindowPixmap()
{
    m_logger.loginfo("Creating client window pixmap");

    if (m_clientWindow)
    {
        m_clientWindowPixmap = XCompositeNameWindowPixmap(
            m_display,
            m_clientWindow);
    }
    if (m_clientWindowPixmap == 0)
    {
        m_logger.logError("Failed to create client window pixmap");
    }
}

void Frame::createFrameDecorationGC()
{
    m_frameDecorationGC = XCreateGC(
        m_display,
        m_frameDecorationPixmap,
        0, nullptr);
    if (m_frameDecorationGC == 0)
    {
        m_logger.logError("Failed to create frame decoration GC");
    }
}

void Frame::createClientWindowGC()
{
}
