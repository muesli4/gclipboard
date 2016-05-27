#ifndef GCLIPBOARD_CLIPBOARD_CONTROLLER_HPP
#define GCLIPBOARD_CLIPBOARD_CONTROLLER_HPP

#include <string>

namespace clipboard
{
    struct controller
    {
        virtual void clipboard_remove(unsigned int id) = 0;
        virtual void clipboard_select_active(unsigned int id) = 0;
        virtual void clipboard_clear() = 0;
        virtual void clipboard_change(unsigned int id, std::string const & s) = 0;
    };
}

#endif

