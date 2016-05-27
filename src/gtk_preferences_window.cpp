#include <gtkmm/stock.h>

#include "gtk_preferences_window.hpp"
#include "gettext.h"

gtk_preferences_window::gtk_preferences_window(preferences::controller & c)
    : _clipboard_table(1, 2)
    , _history_size_label(gettext("History size") + std::string(":"))
    , _cancel_button(Gtk::Stock::CANCEL)
    , _apply_button(Gtk::Stock::APPLY)
{
    this->set_title("Preferences");
    this->set_icon_name("edit-paste");
    this->set_position(Gtk::WIN_POS_CENTER_ALWAYS);
    _history_size_spin_button.set_range(1, 10000);
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
        }
    );

    _clipboard_table.attach(_history_size_label, 0, 1, 0, 1);
    _clipboard_table.attach(_history_size_spin_button, 1, 2, 0, 1);

    _settings_notebook.append_page(_clipboard_table, gettext("Clipboard"));

    _vbox.pack_start(_settings_notebook, false, 2);

    _button_box.pack_end(_cancel_button, false, 2);
    _button_box.pack_end(_apply_button, false, 2);

    _vbox.pack_end(_button_box, false, 2);

    _vbox.show_all();
    this->add(_vbox);
}

void gtk_preferences_window::on_history_size_change(unsigned int new_size)
{
    _history_size_spin_button.set_value(new_size);
}

