#include <algorithm>

#include "model.hpp"

namespace clipboard
{
    model::model()
        : _id_src(0)
    {
    }

    model::~model()
    {
    }

    void model::emit_move_front(unsigned int id)
    {
        emit_view_method(&view::on_move_front, id);
    }

    void model::emit_add(std::string const & s, unsigned int id)
    {
        emit_view_method(&view::on_add, s, id);
    }

    void model::emit_clear()
    {
        emit_view_method(&view::on_clear);
    }

    void model::emit_select_active(unsigned int id)
    {
        emit_view_method(&view::on_select_active, id);
    }

    void model::emit_unselect_active(unsigned int id)
    {
        emit_view_method(&view::on_unselect_active, id);
    }

    void model::emit_remove(unsigned int id)
    {
        emit_view_method(&view::on_remove, id);
    }

    void model::emit_remove_oldest()
    {
        emit_view_method(&view::on_remove_oldest);
    }

    void model::emit_change(unsigned int id, std::string const & s)
    {
        emit_view_method(&view::on_change, id, s);
    }

    void model::emit_freeze(request_type rt)
    {
        emit_view_method(&view::on_freeze, rt);
    }

    void model::emit_thaw()
    {
        emit_view_method(&view::on_thaw);
    }

    unsigned int model::fresh_id()
    {
        auto id = _id_src;
        _id_src += 1;
        return id;
    }
}

