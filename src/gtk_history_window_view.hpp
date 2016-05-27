#ifndef GCLIPBOARD_GTK_HISTORY_WINDOW_VIEW_HPP
#define GCLIPBOARD_GTK_HISTORY_WINDOW_VIEW_HPP

#include <gtkmm/button.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/liststore.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treemodelfilter.h>
#include <gtkmm/window.h>

#include "clipboard/controller.hpp"
#include "clipboard/view.hpp"
#include "freezable/controller.hpp"
#include "freezable/view.hpp"

struct gtk_history_window_view : public Gtk::Window, clipboard::view, freezable::view
{
    gtk_history_window_view(clipboard::controller & cc, freezable::controller & fc);

    ~gtk_history_window_view();

    void on_move_front(unsigned int id);
    void on_select_active(unsigned int id);
    void on_unselect_active(unsigned int id);
    void on_clear();
    void on_add(std::string const & s, unsigned int id);
    void on_remove(unsigned int id);
    void on_remove_oldest();
    void on_change(unsigned int id, std::string const & s);
    void on_freeze(freezable::request_type rt);
    void on_thaw();

    private:

    std::string get_window_title();

    Gtk::ListStore::iterator find_id(unsigned int id);

    Gtk::VBox _vbox;
    Gtk::HBox _button_box;
    Gtk::Button _remove_button;
    Gtk::Button _clear_button;
    Gtk::Button _edit_button;
    Gtk::Button _close_button;

    Gtk::ScrolledWindow _scrolled_window;

    Gtk::SearchEntry _search_entry;

    // the gtk model
    struct history_model_column_record : Gtk::TreeModelColumnRecord
    {
        history_model_column_record();
        Gtk::TreeModelColumn<std::string> entry_column;
        Gtk::TreeModelColumn<std::string> plain_entry_column;
        Gtk::TreeModelColumn<unsigned int> id_column;
    };

    history_model_column_record _column_record;

    Glib::RefPtr<Gtk::ListStore> _list_store_ref;

    Glib::RefPtr<Gtk::TreeModelFilter> _filter_model_ref;
    Glib::ustring _filter_string;

    // the gtk view
    Gtk::ListViewText _list_view_text;

    clipboard::controller & _cc;
    freezable::controller & _fc;

};

#endif

