#ifndef GCLIPBOARD_GTK_PREFERENCES_WINDOW_HPP
#define GCLIPBOARD_GTK_PREFERENCES_WINDOW_HPP

#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/hvbuttonbox.h>
#include <gtkmm/label.h>
#include <gtkmm/notebook.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/grid.h>
#include <gtkmm/window.h>

#include "preferences/controller.hpp"
#include "preferences/view.hpp"

struct gtk_preferences_window : Gtk::Window, preferences::view
{
    gtk_preferences_window(preferences::controller & c);

    private:

    void on_history_size_change(unsigned int new_size);
    void on_session_restore_change(bool restore);

    Gtk::VBox _vbox;

    // notebook for settings
    Gtk::Notebook _settings_notebook;

    Gtk::Grid _clipboard_grid;
    // history-size
    Gtk::Label _history_size_label;
    Gtk::SpinButton _history_size_spin_button;
    // session-restore
    Gtk::Label _session_restore_label;
    Gtk::CheckButton _session_restore_check_button;

    // buttons on the bottom
    Gtk::HButtonBox _button_box;

    Gtk::Button _close_button;
    Gtk::Button _apply_button;
};

#endif

