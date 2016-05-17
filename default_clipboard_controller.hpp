#ifndef DEFAULT_CLIPBOARD_CONTROLLER_HPP
#define DEFAULT_CLIPBOARD_CONTROLLER_HPP

#include <string>

#include "clipboard_model.hpp"
#include "clipboard_controller.hpp"

struct default_clipboard_controller : clipboard_controller
{
    default_clipboard_controller(clipboard_model & m);

    void clipboard_clear();
    void clipboard_select_active(unsigned int id);

    private:
    clipboard_model & _m;
};

#endif

