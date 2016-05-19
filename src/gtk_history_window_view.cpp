#include "gtk_history_window_view.hpp"
#include "gtk_edit_entry_dialog.hpp"
#include "util.hpp"
#include "gettext.h"

#include <iostream>

gtk_history_window_view::gtk_history_window_view(clipboard_controller & ctrl)
    : Gtk::Window()
    , _remove_selected_button(gettext("Remove selected"))
    , _remove_all_button(gettext("Remove all"))
    , _edit_button(gettext("Edit"))
    , _list_view_text(0)
    , _ctrl(ctrl)

{
    this->set_title(gettext("History"));
    this->set_icon_name("edit-paste");
    this->set_default_size(700, 500);

    _list_store_ref = Gtk::ListStore::create(_column_record);

    _scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    _list_view_text.set_model(_list_store_ref);
    _list_view_text.get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

    // TODO make scrollbar decrease again
    //_list_view_text.set_hscroll_policy(Gtk::SCROLL_MINIMUM);

    _list_view_text.append_column(gettext("Entries"), _column_record.entry_column);

    auto selection_ref = _list_view_text.get_selection();
    selection_ref->signal_changed().connect(
        [&, selection_ref]()
        {
            bool sensitive = selection_ref->count_selected_rows() != 0;
            _remove_selected_button.set_sensitive(sensitive);
            _edit_button.set_sensitive(sensitive);
        }
    );

    _scrolled_window.add(_list_view_text);
    
    _vbox.pack_start(_scrolled_window, true, true);

    // controls
    _remove_all_button.signal_released().connect([&](){ _ctrl.clipboard_clear(); });
    _remove_selected_button.set_sensitive(false);
    _remove_selected_button.signal_released().connect(
        [&]()
        {
            std::vector<unsigned int> ids;

            _list_view_text.get_selection()->selected_foreach_iter(
                [&](Gtk::TreeModel::iterator const & it)
                {
                    // note: rows cannot be removed from here
                    ids.push_back((*it)[_column_record.id_column]);
                }
            );

            for (auto id : ids)
                _ctrl.clipboard_remove(id);
        }
    );
    _edit_button.set_sensitive(false);
    _edit_button.signal_released().connect(
        [&]()
        {
            std::vector<std::pair<std::string, unsigned int>> entries;

            // prevent clipboard from changing while we edit it
            _ctrl.clipboard_freeze();

            _list_view_text.get_selection()->selected_foreach_iter(
                [&](Gtk::TreeModel::iterator const & it)
                {
                    // note: rows cannot be removed from here
                    auto & row = *it;
                    entries.emplace_back(std::make_pair(row[_column_record.plain_entry_column], row[_column_record.id_column]));
                }
            );

            if (!entries.empty())
            {
                gtk_edit_entry_dialog dialog(entries);
                dialog.set_transient_for(*this);
                dialog.set_modal();
                int response = dialog.run();

                if (response == Gtk::RESPONSE_APPLY)
                {
                    auto changes = dialog.get_changes();
                    for (auto p : changes)
                    {
                        // TODO what if clipboard changes? freeze it manually?
                        ctrl.clipboard_change(p.second, p.first);
                    }
                }
            }

            _ctrl.clipboard_thaw();
        }
    );

    _button_box.pack_start(_remove_selected_button);
    _button_box.pack_start(_remove_all_button);
    _button_box.pack_end(_edit_button);

    _vbox.pack_end(_button_box, false, false);

    _vbox.show_all();

    this->add(_vbox);
}

gtk_history_window_view::~gtk_history_window_view()
{
}

void gtk_history_window_view::on_move_front(unsigned int id)
{
    auto cs = _list_store_ref->children();

    auto it = find_id(id);

    if (it != cs.end())
    {
        _list_store_ref->iter_swap(it, cs.begin());
    }
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
    row[_column_record.entry_column] = replace_special_whitespace_characters(s);
    row[_column_record.plain_entry_column] = s;
    row[_column_record.id_column] = id;
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
    auto cs = _list_store_ref->children();

    if (!cs.empty())
    {
        _list_store_ref->erase(cs.rbegin().base());
    }
}

void gtk_history_window_view::on_change(unsigned int id, std::string const & s)
{
    auto cs = _list_store_ref->children();

    auto it = find_id(id);

    if (it != cs.end())
    {
        auto & row = *it;
        row[_column_record.plain_entry_column] = s;
        row[_column_record.entry_column] = replace_special_whitespace_characters(s);
    }
}

gtk_history_window_view::history_model_column_record::history_model_column_record()
{
    add(entry_column);
    add(plain_entry_column);
    add(id_column);
}

Gtk::ListStore::iterator gtk_history_window_view::find_id(unsigned int id)
{
    auto cs = _list_store_ref->children();

    return std::find_if(cs.begin(), cs.end(), [&](Gtk::TreeRow const & r){ return r[_column_record.id_column] == id; });
}
