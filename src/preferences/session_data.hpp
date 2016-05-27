#ifndef GCLIPBOARD_PREFERENCES_SESSION_DATA_HPP
#define GCLIPBOARD_PREFERENCES_SESSION_DATA_HPP

#include <string>
#include <vector>

namespace preferences
{
    struct session_data
    {
        unsigned int current_id;
        std::vector<std::pair<unsigned int, std::string>> entries;
        bool active_valid;
        unsigned int active_id;
    };
}

#endif

