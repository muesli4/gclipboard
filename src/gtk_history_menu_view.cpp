#include <utility>
#include <iterator>

#include <glibmm.h>
#include <pangomm.h>

#include "gtk_history_menu_view.hpp"
#include "util.hpp"
#include "gettext.h"

gtk_history_menu_view::gtk_history_menu_view(clipboard::controller & ctrl)
    : _empty_indicator_menu_item(gettext("Empty"))
    , _ctrl(ctrl)
{
    _empty_indicator_menu_item.set_sensitive(false);
    _empty_indicator_menu_item.show();
    this->append(_empty_indicator_menu_item);
}

void gtk_history_menu_view::on_move_front(unsigned int id)
{
    auto it = find_id(id);
    if (it != _menu_items.end())
    {
        // update menu
        this->remove(it->first);
        this->prepend(it->first);

        // update representation
        std::rotate( _menu_items.begin()
                   , it
                   , it + 1
                   );
    }
}

void gtk_history_menu_view::on_select_active(unsigned int id)
{
    auto it = find_id(id);

    if (it != _menu_items.end())
    {
        set_bold_weight(it->first);
    }
}

void gtk_history_menu_view::on_unselect_active(unsigned int id)
{
    auto it = find_id(id);

    if (it != _menu_items.end())
    {
        set_normal_weight(it->first);
    }
}

void gtk_history_menu_view::on_clear()
{
    for (auto & p : _menu_items)
    {
        this->remove(p.first);
    }
    bool empty = _menu_items.empty();
    _menu_items.clear();
    if (!empty)
        show_empty_indicator();
}

void gtk_history_menu_view::on_add(std::string const & s, unsigned int id)
{
    hide_empty_indicator();

    // TODO replace newlines
    // TODO shortened before, because gtk label is slow with long text
    _menu_items.emplace_front(
        std::make_pair( prepare_label_text(s.substr(0, 50))
                      , id
                      )
    );

    Gtk::MenuItem & mi = _menu_items.front().first;

    set_pango_options_to_label(mi);

    mi.signal_activate().connect(
        [this, id]()
        {
            this->_ctrl.clipboard_select_active(id);
        }
    );

    mi.show();
    // add the new item on top
    this->prepend(mi);
}

void gtk_history_menu_view::on_remove(unsigned int id)
{
    auto it = find_id(id);
    if (it != _menu_items.end())
    {
        this->remove(it->first);
        _menu_items.erase(it);
        if (_menu_items.empty())
        {
            show_empty_indicator();
        }
    }
}

void gtk_history_menu_view::on_remove_oldest()
{
    this->remove(_menu_items.back().first);
    _menu_items.pop_back();

    if (_menu_items.empty())
    {
        show_empty_indicator();
    }
}

void gtk_history_menu_view::on_change(unsigned int id, std::string const & s)
{
    auto it = find_id(id);
    if (it != _menu_items.end())
    {
        auto & mi = it->first;
        bool make_bold = has_bold_weight(mi);
        // TODO check for bold ?
        mi.set_label(prepare_label_text(s));

        if (make_bold)
            set_bold_weight(mi);
    }
}

void gtk_history_menu_view::set_pango_options_to_label(Gtk::MenuItem & mi)
{
    auto l = dynamic_cast<Gtk::Label *>(mi.get_child());
    if (l != 0)
    {
        l->set_use_markup();
        l->set_ellipsize(Pango::ELLIPSIZE_END);
        l->set_max_width_chars(40);
    }
}

std::string gtk_history_menu_view::prepare_label_text(std::string s)
{
    return Glib::Markup::escape_text(replace_special_whitespace_characters(s));
}

bool gtk_history_menu_view::has_bold_weight(Gtk::MenuItem & mi)
{
    return mi.get_label()[0] == '<';
}

void gtk_history_menu_view::set_bold_weight(Gtk::MenuItem & mi)
{
    mi.set_label("<b>" + mi.get_label() + "</b>");
}

void gtk_history_menu_view::set_normal_weight(Gtk::MenuItem & mi)
{
    auto const & text = mi.get_label();
    //// FIXME is there some less hacky way to set and remove bold layout on a MenuItem?
    mi.set_label(text.substr(3, text.size() - 7));
}

void gtk_history_menu_view::hide_empty_indicator()
{
    this->remove(_empty_indicator_menu_item);
}

void gtk_history_menu_view::show_empty_indicator()
{
    this->append(_empty_indicator_menu_item);
}

gtk_history_menu_view::buffer_iterator gtk_history_menu_view::find_id(unsigned int id)
{
    return std::find_if(_menu_items.begin(), _menu_items.end(), [id](std::pair<Gtk::MenuItem, unsigned int> const & p){ return p.second == id; });
}

