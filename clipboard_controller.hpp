#ifndef GCLIPBOARD_CLIPBOARD_CONTROLLER_HPP
#define GCLIPBOARD_CLIPBOARD_CONTROLLER_HPP

#include <string>

#include "clipboard_model.hpp"

struct clipboard_controller
{

    virtual void clipboard_select_active(unsigned int id) = 0;
    virtual void clipboard_clear() = 0;
};

#endif

