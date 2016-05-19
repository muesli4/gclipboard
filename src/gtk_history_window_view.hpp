#ifndef GCLIPBOARD_GTK_HISTORY_WINDOW_VIEW_HPP
#define GCLIPBOARD_GTK_HISTORY_WINDOW_VIEW_HPP

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/listviewtext.h>

#include "clipboard_view.hpp"
#include "clipboard_controller.hpp"

struct gtk_history_window_view : public Gtk::Window, clipboard_view
{
    gtk_history_window_view(clipboard_controller & ctrl);

    ~gtk_history_window_view();

    void on_move_front(unsigned int id);
    void on_select_active(unsigned int id);
    void on_unselect_active(unsigned int id);
    void on_clear();
    void on_add(std::string const & s, unsigned int id);
    void on_remove(unsigned int id);
    void on_remove_oldest();
    void on_change(unsigned int id, std::string const & s);

    private:

    Gtk::ListStore::iterator find_id(unsigned int id);

    Gtk::VBox _vbox;
    Gtk::HBox _button_box;
    Gtk::Button _remove_selected_button;
    Gtk::Button _remove_all_button;
    Gtk::Button _edit_button;

    Gtk::ScrolledWindow _scrolled_window;
    // the gtk model
    Gtk::TreeModelColumnRecord _column_record;
    Gtk::TreeModelColumn<std::string> _entry_column;
    // TODO can this be changed via cell renderer?
    Gtk::TreeModelColumn<std::string> _plain_entry_column;
    Gtk::TreeModelColumn<unsigned int> _id_column;

    Glib::RefPtr<Gtk::ListStore> _list_store_ref;

    // the gtk view
    Gtk::ListViewText _list_view_text;


    clipboard_controller & _ctrl;
};

#endif

