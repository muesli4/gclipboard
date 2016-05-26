#include "gtk_enabled_menu_item_view.hpp"


gtk_enabled_menu_item_view::gtk_enabled_menu_item_view(Glib::ustring const & label, clipboard::controller & ctrl)
    : Gtk::CheckMenuItem(label)
    , _ctrl(ctrl)
{
    this->set_active();
    setup_default_signal_toggled_handler();
}

void gtk_enabled_menu_item_view::on_move_front(unsigned int id)
{
}

void gtk_enabled_menu_item_view::on_select_active(unsigned int id)
{
}

void gtk_enabled_menu_item_view::on_unselect_active(unsigned int id)
{
}

void gtk_enabled_menu_item_view::on_clear()
{
}

void gtk_enabled_menu_item_view::on_add(std::string const & s, unsigned int id)
{
}

void gtk_enabled_menu_item_view::on_remove(unsigned int id)
{
}

void gtk_enabled_menu_item_view::on_remove_oldest()
{
}

void gtk_enabled_menu_item_view::on_change(unsigned int id, std::string const & s)
{
}

void gtk_enabled_menu_item_view::on_freeze(clipboard::request_type rt)
{
    set_active_silently(false);
}

void gtk_enabled_menu_item_view::on_thaw()
{
    set_active_silently(true);
}

void gtk_enabled_menu_item_view::setup_default_signal_toggled_handler()
{
    _c = this->signal_toggled().connect(
        [&]()
        {
            if (this->get_active())
            {
                _ctrl.clipboard_thaw(clipboard::request_type::USER);
            }
            else
            {
                _ctrl.clipboard_freeze(clipboard::request_type::USER);
            }
        }
    );
}

void gtk_enabled_menu_item_view::set_active_silently(bool active)
{
    _c.disconnect();
    this->set_active(active);
    setup_default_signal_toggled_handler();
}

