#ifndef GCLIPBOARD_GTK_PREFERENCES_WINDOW_HPP
#define GCLIPBOARD_GTK_PREFERENCES_WINDOW_HPP

#include <gtkmm/button.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/hvbuttonbox.h>
#include <gtkmm/label.h>
#include <gtkmm/notebook.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/table.h>
#include <gtkmm/window.h>

#include "preferences/controller.hpp"
#include "preferences/view.hpp"

struct gtk_preferences_window : Gtk::Window, preferences::view
{
    gtk_preferences_window(preferences::controller & c);


    private:

    void on_history_size_change(unsigned int new_size);

    Gtk::VBox _vbox;

    // notebook for settings
    Gtk::Notebook _settings_notebook;

    Gtk::Table _clipboard_table;
    Gtk::Label _history_size_label;
    Gtk::SpinButton _history_size_spin_button;

    // buttons on the bottom
    Gtk::HButtonBox _button_box;

    Gtk::Button _cancel_button;
    Gtk::Button _apply_button;
};

#endif

