#ifndef GCLIPBOARD_GTK_ENABLED_MENU_ITEM_VIEW_HPP
#define GCLIPBOARD_GTK_ENABLED_MENU_ITEM_VIEW_HPP

#include <gtkmm/checkmenuitem.h>

#include <clipboard/controller.hpp>
#include <clipboard/view.hpp>

struct gtk_enabled_menu_item_view : public Gtk::CheckMenuItem, clipboard::view
{
    gtk_enabled_menu_item_view(Glib::ustring const & label, clipboard::controller & ctrl);

    void on_move_front(unsigned int id);
    void on_select_active(unsigned int id);
    void on_unselect_active(unsigned int id);
    void on_clear();
    void on_add(std::string const & s, unsigned int id);
    void on_remove(unsigned int id);
    void on_remove_oldest();
    void on_change(unsigned int id, std::string const & s);
    void on_freeze(clipboard::request_type rt);
    void on_thaw();

    private:

    void setup_default_signal_toggled_handler();
    void set_active_silently(bool active);

    clipboard::controller & _ctrl;
    sigc::connection _c;
};

#endif

