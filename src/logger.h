#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include "types.h"
#include "util.h"

#define FATAL_LABEL "Fatal: "
#define ERROR_LABEL "Error: "
#define WARN_LABEL "Warn: "
#define INFO_LABEL "Info: "

enum
{
    LOG_LEVEL_NONE,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
};

class Logger
{
public:
    Logger() {}
    ~Logger() {}

    static Logger &getLogger()
    {
        static Logger instance;
        return instance;
    }

public:
    void setLogLevel(int level) {}
    int getLogLevel() { return m_logLevel; }

public:
    void log(int level, std::string msg)
    {
        if (level == LOG_LEVEL_FATAL)
        {
            std::cout << FATAL_LABEL << msg << std::endl;
        }
        if (level == LOG_LEVEL_ERROR)
        {
            std::cout << ERROR_LABEL << msg << std::endl;
        }
        if (level == LOG_LEVEL_WARN)
        {
            std::cout << WARN_LABEL << msg << std::endl;
        }
        if (level == LOG_LEVEL_INFO)
        {
            std::cout << INFO_LABEL << msg << std::endl;
        }
    }

    void logXError(int request, int error, int resourceID, int minorCode)
    {
        std::cerr << "=={ERROR}=================================" << std::endl;
        std::cerr << "Request: " << xErrorRequestCodeToString(request) << std::endl;
        std::cerr << "Error: " << xErrorCodeToString(error) << std::endl;
        std::cerr << "Operation: " << xGetMinorName(request, minorCode) << std::endl;
        std::cerr << "Resource ID: " << resourceID << std::endl;
        std::cerr << "==========================================" << std::endl;
    }
    void logFatal(std::string msg)
    {
        if (m_logLevel >= LOG_LEVEL_FATAL)
            log(LOG_LEVEL_FATAL, msg);
    }
    void logError(std::string msg)
    {
        if (m_logLevel >= LOG_LEVEL_ERROR)
            log(LOG_LEVEL_ERROR, msg);
    }
    void logWarn(std::string msg)
    {
        if (m_logLevel >= LOG_LEVEL_WARN)
            log(LOG_LEVEL_WARN, msg);
    }
    void loginfo(std::string msg)
    {
        if (m_logLevel >= LOG_LEVEL_INFO)
            log(LOG_LEVEL_INFO, msg);
    }

    void logEvent(XEvent &event, int type, WindowInfo &info)
    {
        std::cout << "--{EVENT} { " << xEventTypeToString(type) << " }" << "{ Window ID: " << std::to_string(info.windowPtr) << " }" << std::endl;
        
    }

    void logMapRequestEvent(XMapRequestEvent& event)
    {
        std::cout << "Window Title:" << std::endl;
        std::cout << "Override Redirect:" << std::endl;

    }

public:
    int m_logLevel = LOG_LEVEL_INFO;
};