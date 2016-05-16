#ifndef CLIPBOARD_CONTROLLER_HPP
#define CLIPBOARD_CONTROLLER_HPP

#include <string>

#include "clipboard_model.hpp"

struct clipboard_controller
{

    virtual void select_active_entry(int n) = 0;
    virtual void clear_clipboard_entries() = 0;
};

#endif

