#pragma once
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "types.h"

inline bool safeGetWindowAttributes(Display* dpy, Window w, XWindowAttributes* attr) {
    XSync(dpy, False);
    if (!XGetWindowAttributes(dpy, w, attr))
        return false;
    if (attr->map_state != IsViewable)
        return false;
    return true;
}

inline bool getWindowTypeName(Display *dsp, Window win, std::string& name)
{
    return true;   
}

inline bool getWindowName(Display *dsp, Window win, std::string &windowName)
{
    return true;
}

inline bool isValidWindow(Display* dsp, Window windowPtr)
{

    XWindowAttributes attr;
    if (safeGetWindowAttributes(dsp, windowPtr, &attr))
    {
        return true;
    }

    std::cout << "Invalid window: " << windowPtr << std::endl;

    return false;
}

/// @brief Get Window information from window pointer.
/// @details This function retrieves the window information from the given window pointer (title size ect...).
/// @param dsp Pointer to the display.
/// @param windowPtr Pointer to the window.
/// @param windowInfo refrence to the WindowInfo structure.
inline bool getWindowInfoFromWindow(Display *dsp, Window window, WindowInfo* windowInfo)
{


    if (!true)
    {
        return false;
    }

    windowInfo->windowPtr = window;
    windowInfo->parentWindowPtr = NULL;
    windowInfo->childWindowPtr = NULL;
    windowInfo->rootWindowPtr = NULL;
    windowInfo->title = "";
    windowInfo->posx = 0;
    windowInfo->posy = 0;
    windowInfo->width = 0;
    windowInfo->height = 0;
    windowInfo->mapStatus = MAP_STATUS_UNMAPED;
    windowInfo->depth = 0;
    windowInfo->windowType = "";

    windowInfo->title = "Unknown";
    windowInfo->windowType = "Unknown";

    // Get the attributes of the window
    XWindowAttributes attrs;
    XGetWindowAttributes(dsp, window, &attrs);
    
    // Set the attributes to the WindowInfo structure
    windowInfo->posx = attrs.x;
    windowInfo->posy = attrs.y;
    windowInfo->width = attrs.width;
    windowInfo->height = attrs.height;
    windowInfo->depth = attrs.depth;
    windowInfo->mapStatus = attrs.map_state;
    windowInfo->overrideRedirect = attrs.override_redirect;
    windowInfo->rootWindowPtr = DefaultRootWindow(dsp);
    windowInfo->visual = attrs.visual;
    
    return true;
}

inline bool isRootWindow(Display* dsp , Window windowPtr)
{
    if (!isValidWindow(dsp, windowPtr))
    {
        return false;
    }
    Window rootWindow = RootWindow(dsp, DefaultScreen(dsp));
    if (windowPtr)
    {
        return true;
    }
    return false;
}

inline std::string xEventTypeToString(int type)
    {
        switch (type)
        {
        case 2:
            return "KeyPress";
        case 3:
            return "KeyRelease";
        case 4:
            return "ButtonPress";
        case 5:
            return "ButtonRelease";
        case 6:
            return "MotionNotify";
        case 7:
            return "EnterNotify";
        case 8:
            return "LeaveNotify";
        case 9:
            return "FocusIn";
        case 10:
            return "FocusOut";
        case 11:
            return "KeymapNotify";
        case 12:
            return "Expose";
        case 13:
            return "GraphicsExpose";
        case 14:
            return "NoExpose";
        case 15:
            return "VisibilityNotify";
        case 16:
            return "CreateNotify";
        case 17:
            return "DestroyNotify";
        case 18:
            return "UnmapNotify";
        case 19:
            return "MapNotify";
        case 20:
            return "MapRequest";
        case 21:
            return "ReparentNotify";
        case 22:
            return "ConfigureNotify";
        case 23:
            return "ConfigureRequest";
        case 24:
            return "GravityNotify";
        case 25:
            return "ResizeRequest";
        case 26:
            return "CirculateNotify";
        case 27:
            return "CirculateRequest";
        case 28:
            return "PropertyNotify";
        case 29:
            return "SelectionClear";
        case 30:
            return "SelectionRequest";
        case 31:
            return "SelectionNotify";
        case 32:
            return "ColormapNotify";
        case 33:
            return "ClientMessage";
        case 34:
            return "MappingNotify";
        default:
            return "UnknownEvent(" + std::to_string(type) + ")";
        }
    }

    inline std::string xErrorCodeToString(int error_code)
    {
        switch (error_code)
        {
        case 1:
            return "BadRequest"; // Unknown request code
        case 2:
            return "BadValue"; // Int/enum parameter out of range
        case 3:
            return "BadWindow"; // Invalid window ID
        case 4:
            return "BadPixmap"; // Invalid pixmap ID
        case 5:
            return "BadAtom"; // Invalid Atom ID
        case 6:
            return "BadCursor"; // Invalid cursor ID
        case 7:
            return "BadFont"; // Invalid font ID
        case 8:
            return "BadMatch"; // Parameters mismatch (e.g. depth mismatch)
        case 9:
            return "BadDrawable"; // Invalid drawable (Window/Pixmap)
        case 10:
            return "BadAccess"; // Not allowed to access the resource
        case 11:
            return "BadAlloc"; // Out of memory
        case 12:
            return "BadColor"; // Invalid colormap ID
        case 13:
            return "BadGC"; // Invalid graphics context
        case 14:
            return "BadIDChoice"; // ID already used or invalid
        case 15:
            return "BadName"; // Named color doesn't exist
        case 16:
            return "BadLength"; // Request length doesn't match
        case 17:
            return "BadImplementation"; // Server doesn't implement request
        default:
            return "UnknownError(" + std::to_string(error_code) + ")";
        }
    }

    inline std::string xErrorRequestCodeToString(int request_code)
    {
        switch (request_code)
        {
        case 1:
            return "X_CreateWindow";
        case 2:
            return "X_ChangeWindowAttributes";
        case 3:
            return "X_GetWindowAttributes";
        case 4:
            return "X_DestroyWindow";
        case 5:
            return "X_DestroySubwindows";
        case 6:
            return "X_ChangeSaveSet";
        case 7:
            return "X_ReparentWindow";
        case 8:
            return "X_MapWindow";
        case 9:
            return "X_MapSubwindows";
        case 10:
            return "X_UnmapWindow";
        case 11:
            return "X_UnmapSubwindows";
        case 12:
            return "X_ConfigureWindow";
        case 13:
            return "X_CirculateWindow";
        case 14:
            return "X_GetGeometry";
        case 15:
            return "X_QueryTree";
        case 16:
            return "X_InternAtom";
        case 18:
            return "X_ChangeProperty";
        case 20:
            return "X_DeleteProperty";
        case 23:
            return "X_GetSelectionOwner";
        case 24:
            return "X_SetSelectionOwner";
        case 38:
            return "X_GrabPointer";
        case 40:
            return "X_GrabKeyboard";
        case 45:
            return "X_WarpPointer";
        case 60:
            return "X_AllocColor";
        case 61:
            return "X_AllocNamedColor";
        case 62:
            return "X_AllocColorCells";
        case 63:
            return "X_AllocColorPlanes";
        case 70:
            return "X_PolyPoint";
        case 72:
            return "X_PolyLine";
        case 74:
            return "X_PolyRectangle";
        case 88:
            return "X_GrabButton";
        case 92:
            return "X_GrabKey";
        case 98:
            return "X_QueryExtension";
        case 100:
            return "X_SetInputFocus";
        case 101:
            return "X_GetInputFocus";
        case 102:
            return "X_QueryKeymap";
        case 103:
            return "X_OpenFont";
        case 104:
            return "X_CloseFont";
        case 105:
            return "X_QueryFont";
        case 106:
            return "X_QueryTextExtents";
        case 107:
            return "X_ListFonts";
        case 108:
            return "X_ListFontsWithInfo";
        case 109:
            return "X_SetFontPath";
        case 110:
            return "X_GetFontPath";
        case 130:
            return "X_SetClipRectangles";
        // You can add more request codes here as needed
        default:
            return "UnknownRequest";
        }
    }

    inline std::string xGetMinorName(int major_opcode, int minor_opcode)
    {
        // Composite extension (usually opcode ~139)
        static const std::unordered_map<int, std::string> compositeMinors = {
            {0, "CompositeQueryVersion"},
            {1, "CompositeRedirectWindow"},
            {2, "CompositeRedirectSubwindows"},
            {3, "CompositeUnredirectWindow"},
            {4, "CompositeUnredirectSubwindows"},
            {5, "CompositeCreateRegionFromBorderClip"},
            {6, "CompositeNameWindowPixmap"},
            {7, "CompositeGetOverlayWindow"},
            {8, "CompositeReleaseOverlayWindow"},
        };

        // Render extension (usually opcode ~152)
        static const std::unordered_map<int, std::string> renderMinors = {
            {0, "RenderQueryVersion"},
            {1, "RenderQueryPictFormats"},
            {2, "RenderQueryPictIndexValues"},
            {3, "RenderCreatePicture"},
            {4, "RenderChangePicture"},
            {5, "RenderSetPictureClipRectangles"},
            {6, "RenderFreePicture"},
            {7, "RenderComposite"},
            {8, "RenderScale"},
            {9, "RenderTrapezoids"},
            {10, "RenderTriangles"},
            {11, "RenderTriStrip"},
            {12, "RenderTriFan"},
            {13, "RenderCreateGlyphSet"},
            {14, "RenderReferenceGlyphSet"},
            {15, "RenderFreeGlyphSet"},
            {16, "RenderAddGlyphs"},
            {17, "RenderFreeGlyphs"},
            {18, "RenderCompositeGlyphs8"},
            {19, "RenderCompositeGlyphs16"},
            {20, "RenderCompositeGlyphs32"},
            {21, "RenderFillRectangles"},
            {22, "RenderCreateCursor"},
            {23, "RenderSetPictureTransform"},
            {24, "RenderQueryFilters"},
            {25, "RenderSetPictureFilter"},
            {26, "RenderCreateAnimCursor"},
            {27, "RenderAddTraps"},
            {28, "RenderCreateSolidFill"},
            {29, "RenderCreateLinearGradient"},
            {30, "RenderCreateRadialGradient"},
            {31, "RenderCreateConicalGradient"},
        };

        // XFixes extension (usually opcode ~140)
        static const std::unordered_map<int, std::string> xfixesMinors = {
            {0, "XFixesQueryVersion"},
            {1, "XFixesChangeSaveSet"},
            {2, "XFixesSelectSelectionInput"},
            {3, "XFixesSelectCursorInput"},
            {4, "XFixesGetCursorImage"},
            {5, "XFixesCreateRegion"},
            {6, "XFixesCreateRegionFromBitmap"},
            {7, "XFixesCreateRegionFromWindow"},
            {8, "XFixesCreateRegionFromGC"},
            {9, "XFixesCreateRegionFromPicture"},
            {10, "XFixesDestroyRegion"},
            {11, "XFixesSetRegion"},
            {12, "XFixesCopyRegion"},
            {13, "XFixesUnionRegion"},
            {14, "XFixesIntersectRegion"},
            {15, "XFixesSubtractRegion"},
            {16, "XFixesInvertRegion"},
            {17, "XFixesTranslateRegion"},
            {18, "XFixesRegionExtents"},
            {19, "XFixesFetchRegion"},
            {20, "XFixesSetGCClipRegion"},
            {21, "XFixesSetWindowShapeRegion"},
            {22, "XFixesSetPictureClipRegion"},
            {23, "XFixesSetCursorName"},
            {24, "XFixesGetCursorName"},
            {25, "XFixesGetCursorImageAndName"},
            {26, "XFixesChangeCursor"},
            {27, "XFixesChangeCursorByName"},
            {28, "XFixesExpandRegion"},
        };

        const auto lookup = [&](const auto &table, const std::string &extName) -> std::string
        {
            auto it = table.find(minor_opcode);
            if (it != table.end())
                return extName + "::" + it->second;
            return extName + "::UnknownMinor(" + std::to_string(minor_opcode) + ")";
        };

        switch (major_opcode)
        {
        case 139:
            return lookup(compositeMinors, "Composite");
        case 152:
            return lookup(renderMinors, "Render");
        case 140:
            return lookup(xfixesMinors, "XFixes");
        default:
            return "UnknownMajor(" + std::to_string(major_opcode) + ")::Minor(" + std::to_string(minor_opcode) + ")";
        }
    }