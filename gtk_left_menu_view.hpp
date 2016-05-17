#ifndef CLIPBOARD_GTK_LEFT_MENU_VIEW_HPP
#define CLIPBOARD_GTK_LEFT_MENU_VIEW_HPP

#include <deque>

#include "gtkmm/menu.h"
#include "gtkmm/menuitem.h"

#include "clipboard_view.hpp"
#include "clipboard_controller.hpp"

// TODO maybe remove public inheritance of Menu
struct gtk_left_menu_view : public Gtk::Menu, clipboard_view
{
    gtk_left_menu_view(clipboard_controller & ctrl);

    // methods inherited to implement a view
    void on_select_active(unsigned int id);
    void on_unselect_active(unsigned int id);
    void on_clear();
    void on_add(std::string const & s, unsigned int id);
    void on_remove_oldest();

    private:

    typedef std::deque<std::pair<Gtk::MenuItem, unsigned int>> buffer_type;
    typedef buffer_type::iterator buffer_iterator;

    // TODO factor out into utilities
    static void enable_pango_markup(Gtk::MenuItem & mi);

    void hide_empty_indicator();
    void show_empty_indicator();

    buffer_iterator find_id(unsigned int id);

    Gtk::MenuItem _empty_indicator_menu_item;

    clipboard_controller & _ctrl;

    // new items are inserted into the back
    buffer_type _menu_items;
};

#endif

