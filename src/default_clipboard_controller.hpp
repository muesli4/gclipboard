#ifndef GCLIPBOARD_DEFAULT_CLIPBOARD_CONTROLLER_HPP
#define GCLIPBOARD_DEFAULT_CLIPBOARD_CONTROLLER_HPP

#include <string>

#include "clipboard/model.hpp"
#include "clipboard/controller.hpp"
#include "clipboard/request_type.hpp"

struct default_clipboard_controller : clipboard::controller
{
    default_clipboard_controller(clipboard::model & m);

    void clipboard_clear();
    void clipboard_select_active(unsigned int id);
    void clipboard_remove(unsigned int id);
    void clipboard_change(unsigned int id, std::string const & s);
    void clipboard_freeze(clipboard::request_type rt);
    void clipboard_thaw(clipboard::request_type rt);

    private:
    clipboard::model & _m;
};

#endif

