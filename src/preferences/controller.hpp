#ifndef GCLIPBOARD_PREFERENCES_CONTROLLER_HPP
#define GCLIPBOARD_PREFERENCES_CONTROLLER_HPP

namespace preferences
{
    struct controller
    {
        virtual void preferences_set_history_size(unsigned int new_size) = 0;
    };
}

#endif

