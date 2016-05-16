#include "default_clipboard_controller.hpp"

default_clipboard_controller::default_clipboard_controller(clipboard_model & m)
    : _m(m)
{
}

void default_clipboard_controller::clear_clipboard_entries()
{
    _m.clear_clipboard_entries();
}

void default_clipboard_controller::select_active_entry(int n)
{
    _m.select_active_entry(n);
}
