#ifndef GCLIPBOARD_DEFAULT_CLIPBOARD_CONTROLLER_HPP
#define GCLIPBOARD_DEFAULT_CLIPBOARD_CONTROLLER_HPP

#include <string>

#include "clipboard_model.hpp"
#include "clipboard_controller.hpp"
#include "request_type.hpp"

struct default_clipboard_controller : clipboard_controller
{
    default_clipboard_controller(clipboard_model & m);

    void clipboard_clear();
    void clipboard_select_active(unsigned int id);
    void clipboard_remove(unsigned int id);
    void clipboard_change(unsigned int id, std::string const & s);
    void clipboard_freeze(request_type rt);
    void clipboard_thaw(request_type rt);

    private:
    clipboard_model & _m;
};

#endif

