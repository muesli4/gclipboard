#ifndef GCLIPBOARD_PREFERENCES_VIEW_HPP
#define GCLIPBOARD_PREFERENCES_VIEW_HPP

namespace preferences
{
    struct view
    {
        virtual void on_history_size_change(unsigned int new_size) = 0;
    };
}

#endif

