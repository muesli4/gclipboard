#include <algorithm>

#include "model.hpp"

namespace clipboard {
    model::model()
        : _id_src(0)
    {
    }

    void model::add_view(view & v)
    {
        view * p = &v;
        if (_view_pointers.end() == find_ptr(p))
        {
            _view_pointers.push_back(p);
            init_view(v);
        }
    }

    void model::remove_view(view & v)
    {
        auto it = find_ptr(&v);
        if (_view_pointers.end() == it)
        {
            _view_pointers.erase(it);
        }
    }

    model::~model()
    {
    }

    void model::emit_move_front(unsigned int id)
    {
        for (auto ptr : _view_pointers)
            ptr->on_move_front(id);
    }

    void model::emit_add(std::string const & s, unsigned int id)
    {
        for (auto ptr : _view_pointers)
            ptr->on_add(s, id);
    }

    void model::emit_clear()
    {
        for (auto ptr : _view_pointers)
            ptr->on_clear();
    }

    void model::emit_select_active(unsigned int id)
    {
        for (auto ptr : _view_pointers)
            ptr->on_select_active(id);
    }

    void model::emit_unselect_active(unsigned int id)
    {
        for (auto ptr : _view_pointers)
            ptr->on_unselect_active(id);
    }

    void model::emit_remove(unsigned int id)
    {
        for (auto ptr : _view_pointers)
            ptr->on_remove(id);
    }

    void model::emit_remove_oldest()
    {
        for (auto ptr : _view_pointers)
            ptr->on_remove_oldest();
    }

    void model::emit_change(unsigned int id, std::string const & s)
    {
        for (auto ptr : _view_pointers)
            ptr->on_change(id, s);
    }

    void model::emit_freeze(request_type rt)
    {
        for (auto ptr : _view_pointers)
            ptr->on_freeze(rt);
    }

    void model::emit_thaw()
    {
        for (auto ptr : _view_pointers)
            ptr->on_thaw();
    }

    unsigned int model::fresh_id()
    {
        auto id = _id_src;
        _id_src += 1;
        return id;
    }

    std::vector<view *>::iterator model::find_ptr(view * p)
    {
        return std::find(_view_pointers.begin(), _view_pointers.end(), p);
    }
}

