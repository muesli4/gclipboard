#include "gtk_history_window_view.hpp"
#include "util.hpp"
#include "gettext.h"

gtk_history_window_view::gtk_history_window_view(clipboard_controller & ctrl)
    : Gtk::Window()
    , _remove_selected_button(gettext("Remove selected"))
    , _remove_all_button(gettext("Remove all"))
    , _column_record()
    , _entry_column()
    , _id_column()
    , _list_view_text(0)
    , _ctrl(ctrl)

{
    this->set_title(gettext("History"));
    this->set_icon_name("edit-paste");
    this->set_default_size(700, 500);

    _column_record.add(_entry_column);
    _column_record.add(_id_column);
    _list_store_ref = Gtk::ListStore::create(_column_record);

    _scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    _list_view_text.set_model(_list_store_ref);
    _list_view_text.get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

    // TODO make scrollbar decrease again
    //_list_view_text.set_hscroll_policy(Gtk::SCROLL_MINIMUM);

    _list_view_text.append_column(gettext("Entries"), _entry_column);

    _scrolled_window.add(_list_view_text);
    
    _vbox.pack_start(_scrolled_window, true, true);

    // controls
    _remove_all_button.signal_released().connect([&](){ _ctrl.clipboard_clear(); });
    _remove_selected_button.signal_released().connect(
        [&]()
        {
            std::vector<unsigned int> ids;

            _list_view_text.get_selection()->selected_foreach_iter(
                [&](Gtk::TreeModel::iterator const & it)
                {
                    // note: rows cannot be removed from here
                    ids.push_back((*it)[_id_column]);
                }
            );

            for (auto id : ids)
                _ctrl.clipboard_remove(id);
        }
    );

    _button_box.pack_start(_remove_selected_button);
    _button_box.pack_end(_remove_all_button);

    _vbox.pack_end(_button_box, false, false);

    _vbox.show_all();

    this->add(_vbox);
}

gtk_history_window_view::~gtk_history_window_view()
{
}

void gtk_history_window_view::on_move_front(unsigned int id)
{
    // TODO implement
}

void gtk_history_window_view::on_select_active(unsigned int id)
{
}

void gtk_history_window_view::on_unselect_active(unsigned int id)
{
}

void gtk_history_window_view::on_clear()
{
    _list_store_ref->clear();
}

void gtk_history_window_view::on_add(std::string const & s, unsigned int id)
{
    auto row = *_list_store_ref->prepend();
    row[_entry_column] = replace_special_whitespace_characters(s);
    row[_id_column] = id;
}

void gtk_history_window_view::on_remove(unsigned int id)
{
    auto cs = _list_store_ref->children();

    auto it = find_id(id);

    if (it != cs.end())
    {
        _list_store_ref->erase(it);
    }
}

void gtk_history_window_view::on_remove_oldest()
{
}

Gtk::ListStore::iterator gtk_history_window_view::find_id(unsigned int id)
{
    auto cs = _list_store_ref->children();

    return std::find_if(cs.begin(), cs.end(), [&](Gtk::TreeRow const & r){ return r[_id_column] == id; });
}
