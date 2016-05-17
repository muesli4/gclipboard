#include <vector>
#include <deque>
#include <iostream>

#include <gtkmm/statusicon.h>
#include <gtkmm/application.h>
#include <gtkmm/menu.h>
#include <gtkmm/separatormenuitem.h>
#include <gtkmm/clipboard.h>
#include <gdkmm.h>
#include <glibmm.h>

#include "default_clipboard_controller.hpp"
#include "gtk_clipboard_model.hpp"
#include "gtk_left_menu_view.hpp"

int main(int argc, char ** argv)
{
    auto app = Gtk::Application::create(argc, argv, "org.gclipboard");
    auto status_icon = Gtk::StatusIcon::create("edit-paste");

    gtk_clipboard_model m(10);

    default_clipboard_controller ctrl(m);

    gtk_left_menu_view left_menu(ctrl);

    m.add_view(left_menu);

    Gtk::Menu right_menu;

    Gtk::MenuItem clear_item("Clear");
    clear_item.signal_activate().connect([&](){ ctrl.clipboard_clear(); });
    clear_item.show();
    Gtk::SeparatorMenuItem sep_item;
    sep_item.show();
    Gtk::MenuItem quit_item("Quit");
    quit_item.signal_activate().connect([app](){ app->release(); });
    quit_item.show();

    right_menu.append(clear_item);
    right_menu.append(sep_item);
    right_menu.append(quit_item);


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

