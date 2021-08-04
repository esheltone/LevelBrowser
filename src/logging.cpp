#include "logging.hpp"
#include <map>
#include <string>

std::map<std::string, LoggerContextObject> contextLoggers;
namespace SongBrowser
{
    Logger& Logging::getLogger()
    {
        static Logger* logger = new Logger({ID, VERSION}, LoggerOptions(false, true));
        return *logger;
    }

    LoggerContextObject& Logging::getContextLogger(const std::string_view& context)
    {
        std::string contextString(context);
        std::map<std::string, LoggerContextObject>::iterator it = contextLoggers.find(contextString);
        if (it != contextLoggers.end())
        {
            return it->second;
        }
        contextLoggers.emplace(contextString, getLogger().WithContext(contextString));
        return contextLoggers.find(contextString)->second;
    }
}