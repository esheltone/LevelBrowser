#pragma once
#include "beatsaber-hook/shared/utils/logging.hpp"
#include <string_view>

namespace SongBrowser
{
    class Logging 
    {
        public:
            static Logger& getLogger();
            static LoggerContextObject& getContextLogger(const std::string_view& context);
    };
}

#define INFO(...) SongBrowser::Logging::getContextLogger(__FILE__).info(__VA_ARGS__)
#define ERROR(...) SongBrowser::Logging::getContextLogger(__FILE__).errror(__VA_ARGS__)