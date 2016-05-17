#include <glibmm.h>

#include "gtk_left_menu_view.hpp"
#include "util.hpp"

gtk_left_menu_view::gtk_left_menu_view(clipboard_controller & ctrl)
    : _empty_indicator_menu_item("No items")
    , _ctrl(ctrl)
{
    _empty_indicator_menu_item.set_sensitive(false);
    _empty_indicator_menu_item.show();
    this->append(_empty_indicator_menu_item);
}

void gtk_left_menu_view::on_select_active(unsigned int id)
{
    auto it = find_id(id);

    if (it != _menu_items.end())
    {
        auto & mi = it->first;
        mi.set_label("<b>" + mi.get_label() + "</b>");
    }
}

void gtk_left_menu_view::on_unselect_active(unsigned int id)
{
    auto it = find_id(id);

    if (it != _menu_items.end())
    {
        auto & mi = it->first;
        auto const & text = mi.get_label();
        // FIXME is there some less hacky way to set and remove bold layout on a MenuItem?
        mi.set_label(text.substr(3, text.size() - 7));
    }
}

void gtk_left_menu_view::on_clear()
{
    for (auto & p : _menu_items)
    {
        this->remove(p.first);
    }
    int size = _menu_items.size();
    _menu_items.clear();
    if (size != 0)
        show_empty_indicator();
}

void gtk_left_menu_view::on_add(std::string const & s, unsigned int id)
{
    hide_empty_indicator();

    // TODO trim string to length and replace newlines
    _menu_items.emplace_front(std::make_pair(Glib::Markup::escape_text(replace_special_whitespace_characters(s.substr(0, 20))), id));
    Gtk::MenuItem & mi = _menu_items.front().first;

    enable_pango_markup(mi);

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

void gtk_left_menu_view::on_remove_oldest()
{
    this->remove(_menu_items.back().first);
    _menu_items.pop_back();

    if (_menu_items.empty())
    {
        show_empty_indicator();
    }
}

void gtk_left_menu_view::enable_pango_markup(Gtk::MenuItem & mi)
{
    auto l = dynamic_cast<Gtk::Label *>(mi.get_child());
    if (l != 0)
    {
        l->set_use_markup();
    }
}

void gtk_left_menu_view::hide_empty_indicator()
{
    this->remove(_empty_indicator_menu_item);
}

void gtk_left_menu_view::show_empty_indicator()
{
    this->append(_empty_indicator_menu_item);
}

gtk_left_menu_view::buffer_iterator gtk_left_menu_view::find_id(unsigned int id)
{
    return std::find_if(_menu_items.begin(), _menu_items.end(), [id](std::pair<Gtk::MenuItem, unsigned int> const & p){ return p.second == id; });
}

