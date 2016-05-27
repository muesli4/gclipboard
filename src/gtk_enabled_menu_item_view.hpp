#ifndef GCLIPBOARD_GTK_ENABLED_MENU_ITEM_VIEW_HPP
#define GCLIPBOARD_GTK_ENABLED_MENU_ITEM_VIEW_HPP

#include <gtkmm/checkmenuitem.h>

#include <freezable/controller.hpp>
#include <freezable/view.hpp>

struct gtk_enabled_menu_item_view : public Gtk::CheckMenuItem, freezable::view
{
    gtk_enabled_menu_item_view(Glib::ustring const & label, freezable::controller & ctrl);

    void on_freeze(freezable::request_type rt);
    void on_thaw();

    private:

    void setup_default_signal_toggled_handler();
    void set_active_silently(bool active);

    freezable::controller & _ctrl;
    sigc::connection _c;
};

#endif

