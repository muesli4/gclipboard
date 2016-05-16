#include <vector>
#include <iostream>

#include <gtkmm/statusicon.h>
#include <gtkmm/application.h>
#include <gtkmm/menu.h>
#include <gtkmm/separatormenuitem.h>
#include <gtkmm/clipboard.h>
#include <gdkmm.h>

#include "default_clipboard_controller.hpp"
#include "gtk_clipboard_model.hpp"

// TODO maybe remove public inheritance of Menu
struct LeftMenu : public Gtk::Menu, clipboard_view
{
    LeftMenu(clipboard_controller & ctrl)
        : _ctrl(ctrl)
    {
    }

    void on_select_active_entry(int n)
    {
        // TODO
        std::cout << "view should select entry " << n << std::endl;
    }

    void on_clear_clipboard_entries()
    {
        for (auto & w : _menu_items)
        {
            this->remove(w);
        }
        _menu_items.clear();
    }

    void on_add_clipboard_entry(std::string const & s)
    {
        // TODO trim string to length and remove newlines
        _menu_items.emplace_back(s.substr(0, 20));
        Gtk::MenuItem & mi = _menu_items.back();
        mi.show();
        // add the new item on top
        this->prepend(mi);
    }

    private:

    clipboard_controller & _ctrl;
    std::vector<Gtk::MenuItem> _menu_items;
};

int main(int argc, char ** argv)
{
    auto app = Gtk::Application::create(argc, argv, "org.gclipboard");
    auto status_icon = Gtk::StatusIcon::create("gtk-about");

    Gtk::Menu right_menu;

    Gtk::MenuItem clear_item("Clear");
    clear_item.show();
    Gtk::SeparatorMenuItem sep_item;
    sep_item.show();
    Gtk::MenuItem quit_item("Quit");
    quit_item.signal_activate().connect([app](){ app->release(); });
    quit_item.show();

    right_menu.append(clear_item);
    right_menu.append(sep_item);
    right_menu.append(quit_item);

    gtk_clipboard_model m;

    default_clipboard_controller ctrl(m);

    LeftMenu left_menu(ctrl);

    m.add_view(left_menu);

    status_icon->signal_button_release_event().connect(
        [&, app, status_icon](GdkEventButton * e)
        {
            if (e->button == 1)
            {
                status_icon->popup_menu_at_position(left_menu, 1, e->time);
            }
            else if (e->button == 3)
            {
                status_icon->popup_menu_at_position(right_menu, 3, e->time);
            }
            return true;
        }
    );

    app->hold();
    return app->run(argc, argv);
}

