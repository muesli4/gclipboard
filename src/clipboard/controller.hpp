#ifndef GCLIPBOARD_CLIPBOARD_CONTROLLER_HPP
#define GCLIPBOARD_CLIPBOARD_CONTROLLER_HPP

#include <string>

#include "model.hpp"
#include "request_type.hpp"

namespace clipboard {
    struct controller
    {
        virtual void clipboard_remove(unsigned int id) = 0;
        virtual void clipboard_select_active(unsigned int id) = 0;
        virtual void clipboard_clear() = 0;
        virtual void clipboard_change(unsigned int id, std::string const & s) = 0;
        virtual void clipboard_freeze(request_type rt) = 0;
        virtual void clipboard_thaw(request_type rt) = 0;
    };
}

#endif

