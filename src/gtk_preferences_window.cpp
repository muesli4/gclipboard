#include <gtkmm/stock.h>

#include "gtk_preferences_window.hpp"
#include "gettext.h"

gtk_preferences_window::gtk_preferences_window(preferences::controller & c)
    : _history_size_label(gettext("History size") + std::string(":"))
    , _history_size_spin_button(2, 6)
    , _session_restore_label(gettext("Restore session") + std::string(":"))
    , _button_box()
    , _cancel_button(Gtk::Stock::CANCEL)
    , _apply_button(Gtk::Stock::APPLY)
{
    this->set_title(gettext("Preferences"));
    this->set_icon_name("edit-paste");
    this->set_default_size(500, 300);
    this->set_resizable(false);
    this->set_position(Gtk::WIN_POS_CENTER_ALWAYS);
    _cancel_button.signal_clicked().connect(
        [&]()
        {
            this->hide();
        }
    );

    _apply_button.signal_clicked().connect(
        [&]()
        {
            c.preferences_set_history_size(_history_size_spin_button.get_value());
            c.preferences_set_session_restore(_session_restore_check_button.get_active());
            this->hide();
        }
    );

    _history_size_spin_button.set_digits(0);
    _history_size_spin_button.set_range(1, 10000);
    _history_size_spin_button.set_increments(1, 5);
    _history_size_spin_button.set_numeric();
    _history_size_spin_button.set_halign(Gtk::ALIGN_END);
    _history_size_spin_button.set_hexpand();
    _session_restore_check_button.set_halign(Gtk::ALIGN_END);
    _session_restore_check_button.set_hexpand();

    // clipboard tab
    _clipboard_grid.set_border_width(12);
    _clipboard_grid.set_column_spacing(10);
    _clipboard_grid.set_row_spacing(15);
    // history-size
    _history_size_label.set_halign(Gtk::ALIGN_START);
    _clipboard_grid.attach(_history_size_label, 0, 0, 1, 1);
    _clipboard_grid.attach(_history_size_spin_button, 1, 0, 1, 1);
    // session-restore
    _session_restore_label.set_halign(Gtk::ALIGN_START);
    _clipboard_grid.attach(_session_restore_label, 0, 1, 1, 1);
    _clipboard_grid.attach(_session_restore_check_button, 1, 1, 1, 1);


    _settings_notebook.append_page(_clipboard_grid, gettext("Clipboard"));

    _vbox.pack_start(_settings_notebook, true, true, 2);

    _button_box.set_halign(Gtk::ALIGN_END);
    _button_box.set_homogeneous(true);
    _button_box.set_spacing(2);
    _button_box.pack_start(_cancel_button, false, false, 2);
    _button_box.pack_start(_apply_button, false, false, 2);

    _vbox.pack_end(_button_box, false, false, 2);

    _vbox.show_all();
    this->add(_vbox);
}

void gtk_preferences_window::on_history_size_change(unsigned int new_size)
{
    _history_size_spin_button.set_value(new_size);
}

void gtk_preferences_window::on_session_restore_change(bool restore)
{
    _session_restore_check_button.set_active(restore);
}

