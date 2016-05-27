#include "default_clipboard_controller.hpp"

default_clipboard_controller::default_clipboard_controller(clipboard::model & cm, freezable::model & fm)
    : _cm(cm)
    , _fm(fm)
{
}

void default_clipboard_controller::clipboard_clear()
{
    _cm.clear();
}

void default_clipboard_controller::clipboard_select_active(unsigned int id)
{
    _cm.select_active(id);
}

void default_clipboard_controller::clipboard_remove(unsigned int id)
{
    _cm.remove(id);
}

void default_clipboard_controller::clipboard_change(unsigned int id, std::string const & s)
{
    _cm.change(id, s);
}

void default_clipboard_controller::freezable_freeze(freezable::request_type rt)
{
    _fm.freeze(rt);
}

void default_clipboard_controller::freezable_thaw(freezable::request_type rt)
{
    _fm.thaw(rt);
}

