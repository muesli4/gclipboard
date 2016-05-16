#ifndef DEFAULT_CLIPBOARD_CONTROLLER_HPP
#define DEFAULT_CLIPBOARD_CONTROLLER_HPP

#include <string>

#include "clipboard_model.hpp"
#include "clipboard_controller.hpp"

struct default_clipboard_controller : clipboard_controller
{
    default_clipboard_controller(clipboard_model & m);

    void clear_clipboard_entries();
    void select_active_entry(int n);

    private:
    clipboard_model & _m;
};

#endif

