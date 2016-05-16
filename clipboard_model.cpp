#include <algorithm>

#include "clipboard_model.hpp"

void clipboard_model::add_view(clipboard_view & v)
{
    clipboard_view * p = &v;
    if (_view_pointers.end() == find_ptr(p))
    {
        _view_pointers.push_back(p);
    }
}

void clipboard_model::remove_view(clipboard_view & v)
{
    auto it = find_ptr(&v);
    if (_view_pointers.end() == it)
    {
        _view_pointers.erase(it);
    }
}

void clipboard_model::emit_add_clipboard_entry(std::string const & s)
{
    for (auto ptr : _view_pointers)
        ptr->on_add_clipboard_entry(s);
}

void clipboard_model::emit_clear_clipboard_entries()
{
    for (auto ptr : _view_pointers)
        ptr->on_clear_clipboard_entries();
}

void clipboard_model::emit_select_active_entry(int n)
{
    for (auto ptr : _view_pointers)
        ptr->on_select_active_entry(n);
}

std::vector<clipboard_view *>::iterator clipboard_model::find_ptr(clipboard_view * p)
{
    return std::find(_view_pointers.begin(), _view_pointers.end(), p);
}

