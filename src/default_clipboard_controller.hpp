#ifndef GCLIPBOARD_DEFAULT_CLIPBOARD_CONTROLLER_HPP
#define GCLIPBOARD_DEFAULT_CLIPBOARD_CONTROLLER_HPP

#include <string>

#include "clipboard/model.hpp"
#include "clipboard/controller.hpp"
#include "freezable/model.hpp"
#include "freezable/controller.hpp"
#include "freezable/request_type.hpp"

struct default_clipboard_controller : clipboard::controller, freezable::controller
{
    default_clipboard_controller(clipboard::model & cm, freezable::model & fm);

    void clipboard_clear();
    void clipboard_select_active(unsigned int id);
    void clipboard_remove(unsigned int id);
    void clipboard_change(unsigned int id, std::string const & s);
    void freezable_freeze(freezable::request_type rt);
    void freezable_thaw(freezable::request_type rt);

    private:
    clipboard::model & _cm;
    freezable::model & _fm;
};

#endif

