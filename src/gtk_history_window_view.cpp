#include <gtkmm/stock.h>
#include "gtk_history_window_view.hpp"
#include "gtk_edit_entry_dialog.hpp"
#include "util.hpp"
#include "gettext.h"

// TODO implement key shortcuts for select (enter) and delete (del)
// TODO implement a way to indicate selection: extra column or bold layout

gtk_history_window_view::gtk_history_window_view(clipboard::controller & cc, freezable::controller & fc)
    : Gtk::Window()
    , _remove_button(Gtk::Stock::REMOVE)
    , _clear_button(Gtk::Stock::CLEAR)
    , _edit_button(Gtk::Stock::EDIT)
    , _close_button(Gtk::Stock::CLOSE)
    , _list_view_text(0)
    , _cc(cc)
    , _fc(fc)

{
    this->set_title(get_window_title());
    this->set_icon_name("edit-paste");
    this->set_default_size(700, 500);
    this->set_position(Gtk::WIN_POS_CENTER_ALWAYS);

    _vbox.property_margin() = 2;

    _search_entry.signal_search_changed().connect(
        [&]()
        {
            _filter_string = _search_entry.get_text().lowercase();
            _filter_model_ref->refilter();
        }
    );
    _vbox.pack_start(_search_entry, false, false, 2);

    _list_store_ref = Gtk::ListStore::create(_column_record);

    _filter_model_ref = Gtk::TreeModelFilter::create(_list_store_ref);
    _filter_model_ref->set_visible_func(
        [&](Gtk::TreeModel::const_iterator const & it)
        {
            Glib::ustring target = Glib::ustring((*it)[_column_record.plain_entry_column]);
            return target.lowercase().find(_filter_string) != Glib::ustring::npos;
        }
    );

    _scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    _list_view_text.set_model(_filter_model_ref);
    _list_view_text.get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

    // TODO make scrollbar decrease again
    //_list_view_text.set_hscroll_policy(Gtk::SCROLL_MINIMUM);

    _list_view_text.set_headers_visible(false);
    _list_view_text.append_column("", _column_record.entry_column);
    // TODO use this instead of scrolling?
    // dynamic_cast<Gtk::CellRendererText *>(_list_view_text.get_column_cell_renderer(0))->property_ellipsize() = Pango::ELLIPSIZE_END;

    auto selection_ref = _list_view_text.get_selection();
    selection_ref->signal_changed().connect(
        [&, selection_ref]()
        {
            bool sensitive = selection_ref->count_selected_rows() != 0;
            _remove_button.set_sensitive(sensitive);
            _edit_button.set_sensitive(sensitive);
        }
    );

    _scrolled_window.add(_list_view_text);
    
    _vbox.pack_start(_scrolled_window, true, true, 2);

    // controls
    _clear_button.signal_released().connect([&](){ _cc.clipboard_clear(); });
    _remove_button.set_sensitive(false);
    _remove_button.signal_released().connect(
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
                _cc.clipboard_remove(id);
        }
    );
    _edit_button.set_sensitive(false);
    _edit_button.signal_released().connect(
        [&]()
        {
            std::vector<std::pair<std::string, unsigned int>> entries;

            // prevent clipboard from changing while we edit it
            _fc.freezable_freeze(freezable::request_type::SYSTEM);

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
                        _cc.clipboard_change(p.second, p.first);
                    }
                }
            }

            _fc.freezable_thaw(freezable::request_type::SYSTEM);
        }
    );
    _close_button.signal_released().connect([&](){ this->hide(); });

    _button_box.set_homogeneous(true);
    _button_box.pack_start(_edit_button, true, true, 1);
    _button_box.pack_start(_remove_button, true, true, 1);
    _button_box.pack_start(_clear_button, true, true, 1);
    _button_box.pack_start(_close_button, true, true, 1);

    _vbox.pack_end(_button_box, false, false, 1);

    _vbox.show_all();

    this->add(_vbox);
    this->set_border_width(2);
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
        auto it = cs.end();
        it--;
        _list_store_ref->erase(it);
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

void gtk_history_window_view::on_freeze(freezable::request_type rt)
{
    this->set_title(get_window_title() + " (" + (rt == freezable::request_type::USER ? std::string(gettext("manually")) + ' ' : "") + gettext("frozen") + ")");
}

void gtk_history_window_view::on_thaw()
{
    this->set_title(get_window_title());
}

gtk_history_window_view::history_model_column_record::history_model_column_record()
{
    add(entry_column);
    add(plain_entry_column);
    add(id_column);
}

std::string gtk_history_window_view::get_window_title()
{
    return gettext("History");
}

Gtk::ListStore::iterator gtk_history_window_view::find_id(unsigned int id)
{
    auto cs = _list_store_ref->children();

    return std::find_if(cs.begin(), cs.end(), [&](Gtk::TreeRow const & r){ return r[_column_record.id_column] == id; });
}
