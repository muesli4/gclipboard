#include "default_clipboard_controller.hpp"

default_clipboard_controller::default_clipboard_controller(clipboard_model & m)
    : _m(m)
{
}

void default_clipboard_controller::clipboard_clear()
{
    _m.clear();
}

void default_clipboard_controller::clipboard_select_active(unsigned int id)
{
    _m.select_active(id);
}

void default_clipboard_controller::clipboard_remove(unsigned int id)
{
    _m.remove(id);
}

void default_clipboard_controller::clipboard_change(unsigned int id, std::string const & s)
{
    _m.change(id, s);
}

void default_clipboard_controller::clipboard_freeze()
{
    _m.freeze();
}

void default_clipboard_controller::clipboard_thaw()
{
    _m.thaw();
}

