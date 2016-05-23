#include <algorithm>

#include "clipboard_model.hpp"

clipboard_model::clipboard_model()
    : _id_src(0)
{
}

void clipboard_model::add_view(clipboard_view & v)
{
    clipboard_view * p = &v;
    if (_view_pointers.end() == find_ptr(p))
    {
        _view_pointers.push_back(p);
        init_view(v);
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

clipboard_model::~clipboard_model()
{
}

void clipboard_model::emit_move_front(unsigned int id)
{
    for (auto ptr : _view_pointers)
        ptr->on_move_front(id);
}

void clipboard_model::emit_add(std::string const & s, unsigned int id)
{
    for (auto ptr : _view_pointers)
        ptr->on_add(s, id);
}

void clipboard_model::emit_clear()
{
    for (auto ptr : _view_pointers)
        ptr->on_clear();
}

void clipboard_model::emit_select_active(unsigned int id)
{
    for (auto ptr : _view_pointers)
        ptr->on_select_active(id);
}

void clipboard_model::emit_unselect_active(unsigned int id)
{
    for (auto ptr : _view_pointers)
        ptr->on_unselect_active(id);
}

void clipboard_model::emit_remove(unsigned int id)
{
    for (auto ptr : _view_pointers)
        ptr->on_remove(id);
}

void clipboard_model::emit_remove_oldest()
{
    for (auto ptr : _view_pointers)
        ptr->on_remove_oldest();
}

void clipboard_model::emit_change(unsigned int id, std::string const & s)
{
    for (auto ptr : _view_pointers)
        ptr->on_change(id, s);
}

void clipboard_model::emit_freeze(request_type rt)
{
    for (auto ptr : _view_pointers)
        ptr->on_freeze(rt);
}

void clipboard_model::emit_thaw()
{
    for (auto ptr : _view_pointers)
        ptr->on_thaw();
}

unsigned int clipboard_model::fresh_id()
{
    auto id = _id_src;
    _id_src += 1;
    return id;
}

std::vector<clipboard_view *>::iterator clipboard_model::find_ptr(clipboard_view * p)
{
    return std::find(_view_pointers.begin(), _view_pointers.end(), p);
}

