#ifndef LOGGER_H
#define LOGGER_H

#include <string>

enum class LogLevel {
    DEBUG,
    INFO
};

class Logger {
private:
    static bool debugEnabled;
    
public:
    static void setDebugMode(bool enabled) {
        debugEnabled = enabled;
    }
    
    static bool isDebugEnabled() {
        return debugEnabled;
    }
    
    static void debug(const std::string& message) {
        if (debugEnabled) {
            log(message);
        }
    }
    
    static void info(const std::string& message) {
        log(message);
    }
    
    
private:
    static void log(const std::string& message) {
        std::cout << message << std::endl; 
    }
    
};

inline bool Logger::debugEnabled = false;

#endif 