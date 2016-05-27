#include "gtk_enabled_menu_item_view.hpp"


gtk_enabled_menu_item_view::gtk_enabled_menu_item_view(Glib::ustring const & label, freezable::controller & ctrl)
    : Gtk::CheckMenuItem(label)
    , _ctrl(ctrl)
{
    this->set_active();
    setup_default_signal_toggled_handler();
}

void gtk_enabled_menu_item_view::on_freeze(freezable::request_type rt)
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
                _ctrl.freezable_thaw(freezable::request_type::USER);
            }
            else
            {
                _ctrl.freezable_freeze(freezable::request_type::USER);
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

